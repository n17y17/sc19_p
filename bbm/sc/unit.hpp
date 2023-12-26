#ifndef SC19_PICO_SC_UNIT_HPP_
#define SC19_PICO_SC_UNIT_HPP_

/**************************************************
 * 単位に関するコードです
 * このファイルは，unit.cppに書かれている関数の一覧です
 * 
 * このファイルでは，単位の利用や変換に関するコードが宣言されています．
**************************************************/

//! @file unit.hpp
//! @brief 単位のクラスと変換

#include "sc_basic.hpp"


namespace sc
{

//! @brief 単位がHzの値
class Hz
{
    const float _hz_value;

public:
    //! @brief 単位がHzの値をセットアップ
    //! @param hz_value Hzの値
    Hz(float hz_value);

    operator float() const;
    
    static constexpr float MinHz = 0.0F;   //! 単位がHzの最小の値
    static constexpr float MaxHz = 1000000.0F;   //! 単位がHzの最大の値


};

}

#endif  // SC19_PICO_SC_UNIT_HPP_