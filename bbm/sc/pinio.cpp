#include "pinio.hpp"

/**************************************************
 * GPIOピンの出入力に関するコードです
 * このファイルは，pinio.hppに書かれている関数の中身です
 * 
 * このファイルでは，GPIOのセットアップ，GPIOでの出入力に関するコードが定義されています．
**************************************************/

//! @file pinio.cpp
//! @brief GPIOピンの出入力

namespace sc
{

/***** class Pin *****/

void Pin::set_pull(Pin::Pull pull) const
{
    switch (pull)
    {
        case Pin::Pull::up:
        {
            gpio_pull_up(_pin_gpio);  // pico-SDKの関数  プルアップ抵抗を設定
            break;
        }
        case Pin::Pull::down:
        {
            gpio_pull_down(_pin_gpio);  // pico-SDKの関数  プルダウン抵抗を設定
            break;
        }
        case Pin::Pull::no:
        {
            gpio_disable_pulls(_pin_gpio);  // pico-SDKの関数  プルアップ・プルダウンを無効化
            break;
        }
    }
}


/***** class Pin::In *****/

Pin::In::In(uint8_t pin_gpio):
    Pin::In::In(pin_gpio, Pin::Pull::no)

Pin::In::In(uint8_t pin_gpio, Pin::Pull pull):
    _pin_gpio(pin_gpio)
{
    constexpr uint8_t MinPinGpio = 0;  // GPIOピンの最小の番号
    constexpr uint8_t MaxPinGpio = 28; // GPIOピンの最大の番号

    if (_pin_gpio < MinPinGpio || MaxPinGpio < _pin_gpio)
    {
        throw sc::Error(__FILE__, __LINE__, "Invalid pin_gpio number entered");  // 無効なピンのGPIO番号が入力されました
    }

    ::gpio_init(_pin_gpio);  // pico-SDKの関数  ピンの初期化を行う
    ::gpio_set_dir(_pin_gpio, GPIO_IN);  // pico-SDKの関数  通信方向の設定を行う
    set_pull(pull);  // プルアップ・ダウン抵抗を設定
}

bool Pin::In::read() const
{
    return ::gpio_get(_pin_gpio);  // pico-SDKの関数  ピンがHighになっているかLowになっているかを取得する
}


/***** class Pin::Out *****/

Pin::Out::Out(uint8_t pin_gpio):
    Pin::Out::Out(pin_gpio, Pin::Pull::no)

Pin::Out::Out(uint8_t pin_gpio, Pin::Pull pull):
    _pin_gpio(pin_gpio)
{
    constexpr uint8_t MinPinGpio = 0;  // GPIOピンの最小の番号
    constexpr uint8_t MaxPinGpio = 28; // GPIOピンの最大の番号

    if (_pin_gpio < MinPinGpio || MaxPinGpio < _pin_gpio)
    {
        throw sc::Error(__FILE__, __LINE__, "Outvalid pin_gpio number entered");  // 無効なピンのGPIO番号が入力されました
    }

    ::gpio_init(_pin_gpio);  // pico-SDKの関数  ピンの初期化を行う
    ::gpio_set_dir(_pin_gpio, GPIO_OUT);  // pico-SDKの関数  通信方向の設定を行う
    set_pull(pull);  // プルアップ・ダウン抵抗を設定
}

void Pin::Out::write(bool level) const
{
    gpio_put(_pin_gpio, level);  // pico-SDKの関数  ピンにHighかLowを出力する
}

}