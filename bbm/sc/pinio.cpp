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

/***** class PinIn *****/

PinIn::PinIn(Pin pin):
    PinIn::PinIn(pin, Pull::no) {}

PinIn::PinIn(Pin pin, Pull pull):
    _pin(pin)
{
    ::gpio_init(_pin.get_gpio());  // pico-SDKの関数  ピンの初期化を行う
    ::gpio_set_dir(_pin.get_gpio(), GPIO_IN);  // pico-SDKの関数  通信方向の設定を行う
    _pin.set_pull(pull);  // プルアップ・ダウン抵抗を設定
}

bool PinIn::read() const
{
    return ::gpio_get(_pin.get_gpio());  // pico-SDKの関数  ピンがHighになっているかLowになっているかを取得する
}


/***** class PinOut *****/

PinOut::PinOut(Pin pin):
    PinOut::PinOut(pin, Pull::no) {}

PinOut::PinOut(Pin pin, Pull pull):
    _pin(pin)
{
    ::gpio_init(_pin.get_gpio());  // pico-SDKの関数  ピンの初期化を行う
    ::gpio_set_dir(_pin.get_gpio(), GPIO_OUT);  // pico-SDKの関数  通信方向の設定を行う
    _pin.set_pull(pull);  // プルアップ・ダウン抵抗を設定
}

void PinOut::write(bool level) const
{
    ::gpio_put(_pin.get_gpio(), level);  // pico-SDKの関数  ピンにHighかLowを出力する
}

}