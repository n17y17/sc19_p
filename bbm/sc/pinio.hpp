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

// GPIOピンのプルアップ・プルダウンを行うクラス
class Pin::Pulls
{
    Pulls() = delete;
public:
    //! @brief ピンのプルアップ・プルダウン
    enum Pull
    {
        no,  // プルアップ・ダウンを使用しない
        up,  // プルアップ
        down  //　プルダウン
    };

    //! @brief プルアップ/プルダウンを設定
    //! @param pin プル抵抗の設定を行う対象のGPIO番号
    //! @param pull プルアップ/プルダウンを指定
    static void set_pull(Pin pin, Pin::Pulls::Pull pull);
};

using Pull = Pin::Pulls::Pull;

// ピンによる入力を行うクラス
class Pin::In : Noncopyable
{
    const Pin _pin;  //! 使用するピンのGPIO番号
public:
    //! @brief 入力用ピンをセットアップ
    //! @param pin 入力用ピンのGPIO番号
    In(Pin pin);

    //! @brief 入力用ピンをセットアップ
    //! @param pin 入力用ピンのGPIO番号
    //! @param pull プルアップ/プルダウンを指定
    In(Pin pin, Pull pull);

    //! @brief 入力用ピンから読み込み
    //! @return High(1)かLow(0)か
    bool read() const;
};

// ピンによる出力を行うクラス
class Pin::Out : Noncopyable
{
    const Pin _pin;  //! 使用するピンのGPIO番号
public:
    //! @brief 出力用ピンをセットアップ
    //! @param pin 入力用ピンのGPIO番号
    Out(Pin pin);

    //! @brief 出力用ピンをセットアップ
    //! @param pin 出力用ピンのGPIO番号
    //! @param pull プルアップ/プルダウンを指定
    Out(Pin pin, Pull pull);

    //! @brief 出力用ピンに書き込み
    //! @param level high(1)かlow(0)か
    void write(bool level) const;
};

}

#endif  // SC19_PICO_SC_PINIO_HPP_