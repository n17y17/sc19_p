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

//! @brief ピンによる出入力を行うクラス
class Pin : Noncopyable
{
    Pin() = delete;
    const uint8_t _pin_gpio;  //! 使用するピンのGPIO番号

public:
    //! @brief ピンのプルアップ・プルダウン
    enum Pull
    {
        up,  // プルアップ
        down,  //　プルダウン
        no  // プルアップ・ダウンを使用しない
    };

private:
    //! @brief プルアップ/プルダウンを設定
    //! @param pull プルアップ/プルダウンを指定
    void set_pull(Pull pull) const;

public:
    //! @brief ピンによる入力を行うクラス
    class In : Noncopyable
    {
    public:
        //! @brief 入力用ピンをセットアップ
        //! @param pin_gpio 入力用ピンのGPIO番号
        In(uint8_t pin_gpio);

        //! @brief 入力用ピンをセットアップ
        //! @param pin_gpio 入力用ピンのGPIO番号
        //! @param pull プルアップ/プルダウンを指定
        In(uint8_t pin_gpio, Pull pull);

        //! @brief 入力用ピンから読み込み
        //! @return High(1)かLow(0)か
        bool read() const;
    };

    //! @brief ピンによる出力を行うクラス
    class Out : Noncopyable
    {
    public:
        //! @brief 出力用ピンをセットアップ
        //! @param pin_gpio 入力用ピンのGPIO番号
        Out(uint8_t pin_gpio);

        //! @brief 出力用ピンをセットアップ
        //! @param pin_gpio 出力用ピンのGPIO番号
        //! @param pull プルアップ/プルダウンを指定
        Out(uint8_t pin_gpio, Pull pull);

        //! @brief 出力用ピンに書き込み
        //! @param level High(1)かLow(0)か
        void write(bool level) const;
    };
};

using Pull = Pin::Pull;

}

#endif  // SC19_PICO_SC_PINIO_HPP_