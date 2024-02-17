#include "BNO055_BBM.hpp" //クラス定義

//BNO055::BNO055()とint BNO55::get_BNO055()内で使われている関数を先に定義しておく？

//一応ifndefにしておいた
// #ifndef I2C_PORT i2c1
// #define I2C_PORT i2c1
// #endif

namespace sc 
{

// void accel_init(void);
static int addr = 0x28;
//加速度
// uint8_t accel[6]; // Store data from the 6 acceleration registers
int16_t accelX, accelY, accelZ; // Combined 3 axis data
float f_accelX, f_accelY, f_accelZ; // Float type of acceleration data
uint8_t accel_val = 0x08; // Start register address
//磁気
// uint8_t mag[6];   // magnetometer registers
int16_t magX, magY, magZ;
float f_magX, f_magY, f_magZ;
uint8_t mag_val = 0x0E;
//ジャイロセンサ
// uint8_t gyro[6];
int16_t gyroX, gyroY, gyroZ;
float f_gyroX, f_gyroY, f_gyroZ;
uint8_t gyro_val = 0x14;
//ここまで


//以下class BNO055で定義した関数とかの中身

BNO055::BNO055(const I2C& i2c):
    _i2c(i2c)
{

    // // Configure the I2C Communication
    // // i2c_init(I2C_PORT, 400 * 1000);
    // gpio_set_function(6, GPIO_FUNC_I2C);
    // gpio_set_function(7, GPIO_FUNC_I2C);
    // gpio_pull_up(6);
    // gpio_pull_up(7);

    // Call accelerometer initialisation function
    printf("0\n");
    accel_init();

}

int BNO055::get_BNO055()
{
    //加速度を読み取る
    // i2c_write_blocking(I2C_PORT, addr, &accel_val, 1, true);
    // i2c_read_blocking(I2C_PORT, addr, accel, 6, false);
    Binary accel = _i2c.read_memory(size_t(6), SlaveAddr(addr), MemoryAddr(accel_val));

    accelX = ((accel[1]<<8) | accel[0]);
    accelY = ((accel[3]<<8) | accel[2]);
    accelZ = ((accel[5]<<8) | accel[4]);

    f_accelX = accelX / 100.00;
    f_accelY = accelY / 100.00;
    f_accelZ = accelZ / 100.00;

    //地磁気
    // i2c_write_blocking(I2C_PORT, addr, &mag_val, 1, true);
    // i2c_read_blocking(I2C_PORT, addr, mag, 6, false);
    Binary mag = _i2c.read_memory(size_t(6), SlaveAddr(addr), MemoryAddr(mag_val));

    magX = ((mag[1] << 8) | mag[0]);
    magY = ((mag[3] << 8) | mag[2]);
    magZ = ((mag[5] << 8) | mag[4]);

    f_magX = magX / 16.00;
    f_magY = magY / 16.00;
    f_magZ = magZ / 16.00;
    
    //ジャイロ
    // i2c_write_blocking(I2C_PORT, addr, &gyro_val, 1, true);
    // i2c_read_blocking(I2C_PORT, addr, gyro, 6, false);
    Binary gyro = _i2c.read_memory(size_t(6), SlaveAddr(addr), MemoryAddr(gyro_val));

    gyroX = ((gyro[1] << 8) | gyro[0]);
    gyroY = ((gyro[3] << 8) | gyro[2]);
    gyroZ = ((gyro[5] << 8) | gyro[4]);

    f_gyroX = gyroX / 900.00;
    f_gyroY = gyroY / 900.00;
    f_gyroZ = gyroZ / 900.00;
}


//最初に定義した関数とかの中身

// Initialise Accelerometer Function
void BNO055::accel_init(void){
    printf("1\n");

    // Check to see if connection is correct
    sleep_ms(1000); // Add a short delay to help BNO005 boot up
    uint8_t reg = 0x00;
    uint8_t chipID[1];
    // i2c_write_blocking(I2C_PORT, addr, &reg, 1, true);
    // i2c_read_blocking(I2C_PORT, addr, chipID, 1, false);
    chipID[0] = _i2c.read_memory(size_t(1), SlaveAddr(addr), MemoryAddr(reg)).at(0);
    printf("ID:%d...\n", int(chipID[0]));

    printf("2\n");


//while文を削除した
    if(chipID[0] != 0xA0){
            printf("Chip ID Not Correct - Check Connection!");
    }
        
    printf("3\n");


    // Use internal oscillator
    uint8_t data[2];
    data[0] = 0x3F;
    data[1] = 0x40;
    // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
    _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));

        
    printf("4\n");


    // Reset all interrupt status bits
    data[0] = 0x3F;
    data[1] = 0x01;
    // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
    _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));

    // Configure Power Mode
    data[0] = 0x3E;
    data[1] = 0x00;
    // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
    _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));
    sleep_ms(50);

    // Defaul Axis Configuration
    data[0] = 0x41;
    data[1] = 0x24;
    // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
    _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));

    // Default Axis Signs
    data[0] = 0x42;
    data[1] = 0x00;
    // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
    _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));

    // Set units to m/s^2
    data[0] = 0x3B;
    data[1] = 0b0001000;
    // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
    _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));
    sleep_ms(30);

    // Set operation to AMG(Accel Mag Gyro)
    data[0] = 0x3D;
    data[1] = 0b0111;
    // i2c_write_blocking(I2C_PORT, addr, data, 2, true);
    _i2c.write_memory(Binary(data[1]), SlaveAddr(addr), MemoryAddr(data[0]));
    sleep_ms(100);

        
    printf("5\n");

}

}