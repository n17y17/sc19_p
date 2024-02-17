#include "bbm.hpp"

namespace sc
{

// extern float f_accelX, f_accelY, f_accelZ;

int main()
{
try
{
/***** setup *****/

    // I2C bme_bno_i2c(SDA(6), SCL(7));
    // print("gseivsbvisea\n");
    // BME280 bme280(bme_bno_i2c);
    // BNO055 bno055(bme_bno_i2c);
    // std::cout << bme_bno_i2c.read_memory(size_t(1), SlaveAddr(0x28), MemoryAddr(0x00)) << std::endl;
    // uint8_t data = 0x00 | 0b10000000;
    // uint8_t read_data[1];
    // i2c_write_blocking(i2c1, 0x28, &data, 1, true);
    // i2c_read_blocking(i2c1, 0x28, read_data, 1, false);
    // printf("%d\n", *read_data);
    // print("tttttt");
    // HCSR04 hcsr04(Pin(1), Pin(0));
    SD sd(MISO(0), MOSI(1), SCK(2), Pin(5), Pin(7));
    NJL5513R nj(ADC(Pin(26)), LED(Pin(16)), LED(Pin(17)));

    Motor2 motor(Motor1(PWM(0), PWM(1)), Motor1(PWM(2), PWM(3)));

    Speaker speaker(Pin(6));

    

/***** loop *****/
    while (true)
    {
        // printf("aiueo\n");
        // // std::tuple<sc::Pressure<sc::Unit::Pa>, sc::Humidity<sc::Unit::percent>, sc::Temperature<sc::Unit::K>> bme_data = bme280.read();
        // printf("bno\n");
        // bno055.get_BNO055();
        // // printf("chipID:%hhu\n", bme280.get_chipID());


        // // print("bme temp:%f\n", double(std::get<0>(bme_data)));
        // // print("bme pres:%f\n", double(std::get<1>(bme_data)));
        // // print("bme humi:%f\n", double(std::get<2>(bme_data)));
        // print("bno accel:%f, %f, %f\n", f_accelX, f_accelY, f_accelZ);
        // hcsr_trig.on();
        // sleep(10_us);
        // hcsr_trig.off();
        // Length<Unit::m> hcsr_data = hcsr04.read();
        // printf("hcsr04 : %f\n", static_cast<double>(hcsr_data));
        printf("a");

        nj.red_led().on();
        printf("b");
        // uint16_t result = ::adc_read();
        uint16_t result = nj.read();
        printf("c");
        // std::cout << result << std::endl;
        printf("result:%hu\n", result);

        sleep(1_s);
        nj.green_led().on();
        sleep(0.5_s);
        nj.green_led().off();
        nj.red_led().off();

        motor.forward(1.0);
        speaker.play_starwars();
        

    }

    return 0;
}
catch(const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    std::cout << e.what() << std::endl;
    return 1;
}
}

}

int main()
{
    stdio_init_all();
    printf("init_ok\n");
    sleep_ms(1000);
    sc::main();
}