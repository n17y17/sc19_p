#include "fm.hpp"

namespace sc
{

enum class Fase 
{
    Wait,//待機
    Fall,//落下
    Ldistance,//遠距離
    Sdistance,//近距離
} fase;

int main()
{
    try
    {

    // ************************************************** //
    //                       setup                        //
    // ************************************************** //
        UART uart_twelite(TX(0), RX(1), 115200_hz);  // TWELITEとの通信を行うUART
        GPIO<In> yobi_spresense(Pin(2), Pull::Down);  // Spresenseとの通信を行う予備のピン
        GPIO<Out> yobi_twelite(Pin(3));  // TWELITEに出力する予備の通信ピン
        UART uart_spresense(TX(4), RX(5), 31250_hz);  // Spresenseとの通信を行うUART
        I2C i2c_bme_bno(SDA(6), SCL(7));  // BMEとBNOからの受信を行うI2C
        LED led_green(Pin(8));  // 照度センサ搭載の緑色LED
        // GPIO 9 はSDカード
        Motor1 motor_left(PWM(11), PWM(10));  // 左のモーター
        // GPIO 12~15 はSDカード
        // GPIO16は未使用
        GPIO<In> para_separate(Pin(17), Pull::Up);  // パラシュート分離の検知用ピン (分離したらHigh(1))
        // GPIO18は未使用
        HCSR04 hcsr04(Pin(28), Pin(19));  // 超音波センサHCSR04
        Motor1 motor_right(PWM(20), PWM(21));  // 右のモーター
        Speaker speaker(Pin(22));  // スピーカー
        GPIO<In> usb_conect(Pin(24));  // USBの接続を確認
        LED led_pico(Pin(25));  // pico内蔵LED
        LED led_red(Pin(27));    // 照度センサ搭載の赤色LED
        NJL5513R njl5513r(ADC(26), led_red, led_green);  // 照度センサnjl5513r
        PicoTemp pico_temp;
        VsysVoltage vsys;
        BME280 bme280(i2c_bme_bno);  // 温湿度気圧センサのBME280
        BNO055 bno055(i2c_bme_bno);  // 9軸センサのBNO055;
        Motor2 motor(motor_left, motor_right);  // 左右のモーター
        SD sd;  // SDカード
        Flush flush;
        Spresense spresense(uart_spresense);
        Twelite twelite(uart_twelite);

        // USBでの接続時はフラッシュメモリのデータを出力
        if (usb_conect.read() == true)
        {
            flush.print();
            #ifndef NODEBUG
                flush.clear();
            #endif
        } else {
            flush.clear();  // フラッシュメモリを削除(削除しないと書き込めない)
        }

        // print関数を設定
        set_print = [&](const std::string & message)
        {
            try {std::cout << message << std::flush;} catch(const std::exception& e){printf(e.what());}
            try {flush.write(message);} catch(const std::exception& e){printf(e.what());}
            try {sd.write(message);} catch(const std::exception& e){printf(e.what());}
            try {twelite.write(0x00, message);} catch(const std::exception& e){printf(e.what());}
        };

    // ************************************************** //
    //                        loop                        //
    // ************************************************** //
        while (true) { 
        try {
        while (true)
        {
            try
            {
                switch (fase)
                {
                    // ************************************************** //
                    //                     待機フェーズ                    //
                    // ************************************************** //
                    case Fase::Wait:
                    {
                        try
                        {
                            auto njl_data = njl5513r.read();
                            if(njl_data>100_lx)//照度によりキャリア展開検知　→落下フェーズへ
                            {
                                fase=Fase::Fall;
                                break;
                            }
                            else
                            {
                                break;
                            }
                        }
                        catch(const std::exception& e){print(e.what());}
                        
                    }

                    // ************************************************** //
                    //                     落下フェーズ                    //
                    // ************************************************** //
                    case Fase::Fall:
                    {
                        try
                        {
                            int separate;
                            auto bme_data = bme280.read();  // BME280(温湿圧)から受信
                            Pressure<Unit::Pa> pressure = std::get<0>(bme_data);  // 気圧
                            Temperature<Unit::degC> temperature = std::get<2>(bme_data);  // 気温
                            Altitude<Unit::m> altitude(pressure, temperature);
                            if(altitude<10.0_m)//地面からの標高が10m以内
                            {
                                sleep(10_s);//念のため待機しておく
                                separate=para_separate.read();
                                if(separate!=1)//パラシュートが取れていない場合、動いてみる？
                                {
                                    motor.forward(1.0);
                                    sleep(1_s);
                                    motor.forward(0);
                                    break;
                                }
                                auto bno_data = bno055.read();  // BNO055(9軸)から受信
                                Acceleration<Unit::m_s2> gravity_acceleration = std::get<1>(bno_data);//重力加速度取得
                                //Z軸の重力加速度が正かどうかで機体の体制修正
                                if(gravity_acceleration.z()>0_m_s2)//z軸が正なら正常
                                {
                                    fase=Fase::Ldistance;
                                    break;
                                } else {
                                    motor.forward(1.0);//じたばたして体制修正できるかな？
                                    sleep(1_s);
                                    motor.forward(0);
                                    motor.right(1.0); 
                                    sleep(1_s);
                                    motor.right(0); 
                                    motor.left(1.0); 
                                    sleep(1_s);
                                    motor.left(0);
                                    break;
                                }
                            } else {
                                break;
                            }
                        }
                        catch(const std::exception& e){print(e.what());}
                    }

                    // ************************************************** //
                    //                   遠距離フェーズ                    //
                    // ************************************************** //
                    case Fase::Ldistance:
                    {
                        try
                        {
                            //------ちゃんと動くか確認するためのコード-----
                            Vector3<double> magnetic(0.0,0.0,0.0);
                            double t_lon = 0.0000;//ゴールの経度
                            double t_lat = 0.0000;//ゴールの緯度
                            double m_lon = 0.0000;//自分の経度
                            double m_lat = 0.0000;//自分の緯度
                            double t_lon_rad = Angle<Unit::deg>::deg_to_rad(t_lon);
                            double t_lat_rad = Angle<Unit::deg>::deg_to_rad(t_lat);
                            double m_lon_rad = Angle<Unit::deg>::deg_to_rad(m_lon);
                            double m_lat_rad = Angle<Unit::deg>::deg_to_rad(m_lat);
                            //-------------------------------------------

                            //機体の正面のベクトルを作る.ただし、bnoの都合上、後ろがxの正の向きで左がyの正の向き、下がzの正の向きとなっている
                            // Vector3<double> front_vetor_basic = (-1.0, 0.0, 0.0);//機体正面の単位ベクトル

                            //北を見つける
                            Vector3<double> North_vector(magnetic.x(),magnetic.y(),0);//磁気センサから求める北の向き
                            // Vector3<double> North_vector_basic = Normalization(North_vector);//正規化
                            double North_angle_rad;//後で使う北の角度

                            // 北がBnoの座標軸において何度回転した位置にあるか求める
                            // 但しθは[0,2Pi)とした
                            North_angle_rad = atan2(magnetic.y(),magnetic.x());
                            if(North_angle_rad < 0)
                            {
                                North_angle_rad += 2 * PI;
                            }

                            printf("%f\n",North_angle_rad);
                            
                            //スタート地点からゴールまでのベクトルを求める
                            //--------------一旦動くかわからんが書いてみる--------------

                            double distance = distance_sphere(t_lon_rad,t_lat_rad,m_lon_rad,m_lat_rad);//ゴールと自分の距離を求める
                            double distance_vertical = distance_sphere(m_lon_rad,t_lat_rad,m_lon_rad,m_lat_rad);//縦の距離を求める(経度を同じにすることで)
                            double distance_horizontal = distance_sphere(t_lon_rad,m_lat_rad,m_lon_rad,m_lat_rad);//横の長さを求める(緯度を同じにすることで)

                            //自分の緯度のほうが高い場合、南向きに進む必要があるため、－符号をかける(北が正のため)
                            if(m_lat_rad > t_lat_rad)
                            {
                                distance_vertical = -1*distance_vertical;
                                return distance_vertical;
                            }
                            //自分の経度のほうが小さい場合、東に向かう必要があるため(以下略)
                            if(m_lon_rad < t_lon_rad)
                            {
                                distance_horizontal = -1*distance_horizontal;
                                return distance_horizontal;
                            }

                            Vector3<double> direction(distance_vertical,distance_horizontal,0);
                            //----------------------------

                            Vector3<double> direction_vector_1(direction.x(),direction.y(),0);//東西南北を基底としたベクトルでベクトルを表現(北がx軸,西がy軸)
                            Vector3<double> direction_vector_2 = Rotation_counter_xy(direction_vector_1,Latitude<sc::Unit::rad>(North_angle_rad));//東西南北の基底から機体のxyを基底とした座標に回転.
                            double direction_angle_rad;

                            direction_angle_rad = atan2(direction_vector_2[1],direction_vector_2[0]);
                            direction_angle_rad = direction_angle_rad - PI;//正面がxの負の向きなので180°回転
                            if(direction_angle_rad < 0)
                            {
                                direction_angle_rad += 2 * PI;
                            }

                            double direction_angle_degree = Angle<Unit::deg>::rad_to_deg(direction_angle_rad);

                            //ここからdirection_angleをもとに機体を動かす
                            //一旦SC-17のコードを引っ張ってきたよ
                            //sleep_msよりsleep使ったほうがいい?
                            if(direction_angle_degree < 45 || direction_angle_degree > 315)
                            {
                                printf("forward\n");
                                // forward(1);
                                // sleep_ms(3000);
                                // forward(0.6);
                                // sleep_ms(1000);
                                // forward(0.2);
                                // sleep_ms(1000);
                                // forward(0);
                                // sleep_ms(100);
                                motor.forward(1.0);
                                sleep_ms(3000);
                                motor.forward(0.6);
                                sleep_ms(1000);
                                motor.forward(0.2);
                                sleep_ms(1000);       
                                motor.forward(0);
                                sleep_ms(100);
                                break;
                            }else if((direction_angle_degree) <135){
                                printf("right\n");
                                // right(0.5);
                                // sleep_ms(300);
                                // right(0);
                                // sleep_ms(100);
                                motor.right(0.5);
                                sleep_ms(300);
                                motor.right(0);
                                sleep_ms(100);
                                break;
                            }else if(direction_angle_degree < 180){
                                printf("sharp_right\n");
                                // right(0.5);
                                // sleep_ms(600);
                                // right(0);
                                // sleep_ms(100);
                                motor.right(0.5);
                                sleep_ms(600);
                                motor.right(0);
                                sleep_ms(100);
                                break;
                            }else if(direction_angle_degree > 225){
                                printf("left\n");
                                // left(0.5);
                                // sleep_ms(300);
                                // left(0);
                                // sleep_ms(100);
                                motor.left(0.5);
                                sleep_ms(300);
                                motor.left(0);
                                sleep_ms(100);
                                break;
                            }else{
                                printf("sharp_left\n");
                                // left(0.5);
                                // sleep_ms(600);
                                // left(0);
                                // sleep_ms(100);
                                motor.left(0.5);
                                sleep_ms(600);
                                motor.left(0);
                                sleep_ms(100);
                                break;
                            }
                        }
                        catch(const std::exception& e){print(e.what());}
                    }

                    // ************************************************** //
                    //                   遠距離フェーズ                    //
                    // ************************************************** //
                    case Fase::Sdistance:
                    {
                        try
                        {
                            if(true)//ゴールがカメラの真ん中
                            {
                                //少し進む
                                motor.forward(1.0);
                                sleep(1_s);
                                motor.forward(0);
                                //超音波でゴール検知　→ゴール    
                                double hcsr_data_average = 0;
                                for(int i=0;i<10;++i)//超音波の値の平均
                                {
                                    hcsr_data_average += double(hcsr04.read());
                                }
                                hcsr_data_average/=10.0;
                                if(hcsr_data_average<0.2)//0.2m以内でゴール
                                {
                                    print("goal\n");
                                    while(true)
                                    {
                                        ;
                                    }
                                }
                            }
                            else if(true)//ゴールがカメラの右
                            {
                                motor.right(1.0); 
                                sleep(1_s);
                                motor.right(0); 
                                break;
                            }
                            else if(true)//ゴールがカメラの左
                            {
                                motor.left(1.0); 
                                sleep(1_s);
                                motor.left(0); 
                                break;
                            }
                            else//ゴールがみつからない
                            {
                                motor.right(1.0); 
                                sleep(1_s);
                                motor.right(0); 
                                break;
                            }
                        }
                        catch(const std::exception& e){print(e.what());}
                    }

                    // ************************************************** //
                }
            }
            catch(const std::exception& e){print(e.what());}
        }
        } catch(const std::exception& e){print(e.what());}
        }
    }
    catch(const std::exception& e)
    {
        // エラー時の出力
        print(e.what());
        print("\nOut of the loop\n");
        sleep(100_ms);
        exit(EXIT_FAILURE);
    }
}


}  // namespace sc

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    printf("init_ok\n");
    sc::main();
}