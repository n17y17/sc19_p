#ifndef SC19_PICO_SC_PINIO_HPP_
#define SC19_PICO_SC_PINIO_HPP_

/**************************************************
 * GPIOピンの出入力に関するコードです
 * このファイルは，pinio.cppに書かれている関数の一覧です
 * 
 * このファイルでは，GPIOのセットアップ，GPIOでの出入力に関するコードが宣言されています．
**************************************************/

//! @file pinio.hpp
//! @brief GPIOピンの出入力

#include "sc_basic.hpp"

#include "hardware/gpio.h"

namespace sc
{

//! @brief ピンによる入力を行うクラス
class PinIn
{
    const Pin _pin;  //! 使用するピンのGPIO番号
public:
    //! @brief 入力用ピンをセットアップ
    //! @param pin 入力用ピンのGPIO番号
    PinIn(Pin pin);

    //! @brief 入力用ピンをセットアップ
    //! @param pin 入力用ピンのGPIO番号
    //! @param pull プルアップ/プルダウンを指定
    PinIn(Pin pin, Pull pull);

    //! @brief 入力用ピンから読み込み
    //! @return High(1)かLow(0)か
    bool read() const;
};

//! @brief ピンによる出力を行うクラス
class PinOut
{
    const Pin _pin;  //! 使用するピンのGPIO番号
public:
    //! @brief 出力用ピンをセットアップ
    //! @param pin 入力用ピンのGPIO番号
    PinOut(Pin pin);

    //! @brief 出力用ピンをセットアップ
    //! @param pin 出力用ピンのGPIO番号
    //! @param pull プルアップ/プルダウンを指定
    PinOut(Pin pin, Pull pull);

    //! @brief 出力用ピンに書き込み
    //! @param level high(1)かlow(0)か
    void write(bool level) const;
};

}

#endif  // SC19_PICO_SC_PINIO_HPP_