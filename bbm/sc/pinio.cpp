#include "pinio.hpp"

/**************************************************
 * GPIOピンの出入力に関するコードです
 * このファイルは，pinio.hppに名前だけ書かれている関数の中身です
 * 
 * このファイルでは，GPIOのセットアップ，GPIOでの出入力に関するコードが定義されています．
**************************************************/

//! @file pinio.cpp
//! @brief GPIOピンの出入力

namespace sc
{

/***** class Pin::Pull *****/

void Pin::Pulls::set_pull(Pin pin, Pull pull)
{
    switch (pull)
    {
        case Pull::up:
        {
            ::gpio_pull_up(pin.get_gpio());  // pico-SDKの関数  プルアップ抵抗を設定
            break;
        }
        case Pull::down:
        {
            ::gpio_pull_down(pin.get_gpio());  // pico-SDKの関数  プルダウン抵抗を設定
            break;
        }
        case Pull::no:
        {
            ::gpio_disable_pulls(pin.get_gpio());  // pico-SDKの関数  プルアップ・プルダウンを無効化
            break;
        }
    }
}


/***** class Pin::In *****/

Pin::In::In(Pin pin):
    Pin::In::In(pin, Pull::no) {}

Pin::In::In(Pin pin, Pull pull):
    _pin(pin)
{
    ::gpio_init(_pin.get_gpio());  // pico-SDKの関数  ピンの初期化を行う
    ::gpio_set_dir(_pin.get_gpio(), GPIO_IN);  // pico-SDKの関数  通信方向の設定を行う
    Pin::Pulls::set_pull(_pin, pull);  // プルアップ・ダウン抵抗を設定
}

bool Pin::In::read() const
{
    return ::gpio_get(_pin.get_gpio());  // pico-SDKの関数  ピンがHighになっているかLowになっているかを取得する
}


/***** class Pin::Out *****/

Pin::Out::Out(Pin pin):
    Pin::Out::Out(pin, Pull::no) {}

Pin::Out::Out(Pin pin, Pull pull):
    _pin(pin)
{
    ::gpio_init(_pin.get_gpio());  // pico-SDKの関数  ピンの初期化を行う
    ::gpio_set_dir(_pin.get_gpio(), GPIO_OUT);  // pico-SDKの関数  通信方向の設定を行う
    Pin::Pulls::set_pull(pin, pull);  // プルアップ・ダウン抵抗を設定
}

void Pin::Out::write(Pin::Out::Level level) const
{
    ::gpio_put(_pin.get_gpio(), level);  // pico-SDKの関数  ピンにHighかLowを出力する
}

}