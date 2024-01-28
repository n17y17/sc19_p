/**************************************************
 * PWMに関するコードです
 * このファイルは，pwm.hppに名前だけ書かれている関数の中身です
 * 
 * このファイルでは，PWMによる出力に関するコードが定義されています．
**************************************************/

//! @file pwm.cpp
//! @brief PWMによる出力

#include "pwm.hpp"

#include "hardware/pwm.h"

namespace sc
{

/***** class Duty *****/

constexpr Duty::Duty(float duty):
    _duty(duty)
{
    if (_duty < MinDuty || MaxDuty < _duty)
    {
throw Error(__FILE__, __LINE__, "An incorrect Duty value was entered");  // 正しくないDuty比が入力されました
    }
}


/***** class PWM *****/

PWM::PWM(Pin pin, Freq freq):
    _pin(pin), _slice(::pwm_gpio_to_slice_num(pin.gpio())), _channel(::pwm_gpio_to_channel(pin.gpio()) == 1 ? Channel::B : Channel::A), _wrap(to_wrap(freq)), _clk_div(to_clk_div(freq, to_wrap(freq)))
{
    if (Pin::Status.at(_pin.gpio()) != PinStatus::NoUse)
    {
throw Error(__FILE__, __LINE__, "This pin is already in use");  // このピンは既に使用されています
    }

    ::gpio_set_function(_pin.gpio(), GPIO_FUNC_PWM);  // pico-SDKの関数  ピンの機能をPWMにする

    // 周波数を設定 (実際に設定される周波数は入力した値から最大で6.25%ずれる)
    ::pwm_set_wrap(_slice, _wrap);  // pico-SDKの関数  分解能を設定 (詳しくは下記の資料へ)
    ::pwm_set_clkdiv(_slice, _clk_div);  // pico-SDKの関数  分周比を設定 (詳しくは下記の資料へ)

    ::pwm_set_phase_correct(_slice, false);  // pico-SDKの関数  三角波にするならtrue
    ::pwm_set_output_polarity(_slice, false, false);  // pico-SDKの関数  チャンネルAとBの出力値の0と1を反転させるなら，それぞれtrue

    ::pwm_set_enabled(_slice, true);  // pico-SDKの関数  PWMをオンにする
}

void PWM::write(Duty duty) const
{
    ::pwm_set_gpio_level(_pin.gpio(), _wrap * duty);  // pico-SDKの関数  あるGPIOピンのPWMの出力レベルを設定する
    // ::pwm_set_chan_level(_slice, (_channel==Channel::A ? PWM_CHAN_A : PWM_CHAN_B), _wrap * duty);  // pico-SDKの関数  sliceとchannelで指定したGPIOピンのPWMの出力レベルを設定する
}

void PWM::write(_ms high_time) const
{
    ::pwm_set_gpio_level(_pin.gpio(), static_cast<double>(static_cast<_s>(high_time))*SysClock*SysClock/(_clk_div*_clk_div*(_wrap+1)));  // pico-SDKの関数  あるGPIOピンのPWMの出力レベルを設定する
    // ::pwm_set_chan_level(_slice, (_channel==Channel::A ? PWM_CHAN_A : PWM_CHAN_B), static_cast<double>(static_cast<_s>(high_time))*SysClock*SysClock/(_clk_div*_clk_div*(_wrap+1)));  // pico-SDKの関数  sliceとchannelで指定したGPIOピンのPWMの出力レベルを設定する
}

uint16_t PWM::to_wrap(Freq freq)
{
    if (freq < MinFreq || MaxFreq < freq)
    {
throw Error(__FILE__, __LINE__, "Invalid frequency entered");  // 無効な周波数が入力されました
    }

    if ((SysClock / not0(static_cast<double>(freq))) > (0xffff + 1))  // 分解能はできる限り高くなるように設定
    {
return 0xffff;
    } else {
return (SysClock / not0(static_cast<double>(freq))) - 1;
    }
}

float PWM::to_clk_div(Freq freq, uint16_t wrap)
{
    if (freq < MinFreq || MaxFreq < freq)
    {
throw Error(__FILE__, __LINE__, "Invalid frequency entered");  // 無効な周波数が入力されました
    } else if (wrap < MinWrap || MaxWrap < wrap) {
throw Error(__FILE__, __LINE__, "Invalid wrap entered");  // 無効なWrapが入力されました
    }

    float clk_div = SysClock / not0(static_cast<double>(freq) * (wrap + 1));  // 分解能の値をもとに分周比の値を計算
    clk_div = static_cast<float>(static_cast<uint16_t>(clk_div * 16.0F)) / 16.0F;  // 分周比を 1/16 の倍数に直す
    return clk_div;
}

// PWMクラスは以下の資料を参考にして作成しました
// https://lipoyang.hatenablog.com/entry/2021/12/12/201432
// https://rikei-tawamure.com/entry/2021/02/08/213335

}