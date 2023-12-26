#ifndef SC19_PICO_SC_I2C_HPP_
#define SC19_PICO_SC_I2C_HPP_

/**************************************************
 * I2C通信の出入力に関するコードです
 * このファイルは，i2c.cppに書かれている関数の一覧です
 * 
 * このファイルでは，I2Cのマスター側の通信に関するコードが宣言されています．
**************************************************/

//! @file i2c.hpp
//! @brief I2Cでの入出力

#include "sc_basic.hpp"

#include "hardware/i2c.h"


namespace sc
{

//! @brief I2C通信による出入力
class I2C : Noncopyable
{
public:
    //! @brief  I2CのSDAピンを指定
    class SDA
    {
        Pin _sda_pin;  // sdaピンのGPIO番号
    public:
        //! @brief SDAピンを指定
        //! @param sda_pin SDAピンのGPIO番号
        SDA(Pin sda_pin):
            _sda_pin(sda_pin) {}

        //! @brief SDAピンを指定
        //! @param sda_gpio SDAピンのGPIO番号
        SDA(uint8_t sda_gpio):
            SDA(Pin(sda_gpio)) {}
    };

    //! @brief  I2CのSCLピンを指定
    class SCL
    {
        Pin _scl_pin;  // sclピンのGPIO番号
    public:
        //! @brief SCLピンを指定
        //! @param scl_pin SCLピンのGPIO番号
        SCL(Pin scl_pin):
            _scl_pin(scl_pin) {}

        //! @brief SCLピンを指定
        //! @param scl_gpio SCLピンのGPIO番号
        SCL(uint8_t scl_gpio):
            SCL(Pin(scl_gpio)) {}
    };


    //! @brief  I2Cの通信速度を指定
    class Freq
    {
    //     Pin _scl_pin;  // sclピンのGPIO番号
    // public:
    //     //! @brief SCLピンを指定
    //     //! @param scl_pin SCLピンのGPIO番号
    //     SCL(Pin scl_pin):
    //         _scl_pin(scl_pin) {}

    //     //! @brief SCLピンを指定
    //     //! @param scl_gpio SCLピンのGPIO番号
    //     SCL(uint8_t scl_gpio):
    //         SCL(Pin(scl_gpio)) {}
    };

private:
    const SDA _sda_pin;  // I2Cで使用するSDAピン
    const SCL _scl_pin;  // I2Cで使用するSCLピン
    const Freq _freq;  // I2Cの通信速度

public:
    //! @brief I2Cをセットアップ
    //! @param sda_pin I2Cで使用するSDAピン
    //! @param scl_pin i2Cで使用するSCLピン
    I2C(SDA sda_pin, SCL scl_pin, Freq freq);

    //! @brief I2Cをセットアップ
    //! @param sda_pin I2Cで使用するSDAピン
    //! @param scl_pin i2Cで使用するSCLピン
    I2C(SDA sda_pin, SCL scl_pin);
};

using SDA = I2C::SDA;
using SCL = I2C::SCL;


}

#endif  // SC19_PICO_SC_I2C_HPP_