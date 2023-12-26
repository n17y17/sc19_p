/**************************************************
 * 単位にかかわるコードです
 * このファイルは，unit.hppに名前だけ書かれている関数の中身です
 * 
 * このファイルでは，単位の利用や変換に関するコードが定義されています．
**************************************************/

//! @file unit.cpp
//! @brief 単位のクラスと変換

#include "unit.hpp"

namespace sc
{

/***** class Hz *****/

Hz::Hz(float hz_value):
    _hz_value(hz_value)
{
    if (_hz_value < MinHz || MaxHz < _hz_value)
    {
        
    }
}

}