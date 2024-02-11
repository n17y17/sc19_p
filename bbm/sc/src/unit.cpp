// /**************************************************
//  * 単位にかかわるコードです
//  * このファイルは，unit.hppに名前だけ書かれている関数の中身です
//  * 
//  * このファイルでは，単位の利用や変換に関するコードが定義されています．
// **************************************************/

// //! @file unit.cpp
// //! @brief 単位のクラスと変換

#include "sc_basic.hpp"

#include <cmath>

#include "unit.hpp"

// namespace sc
// {

// /***** class _degC *****/

// _degC::_degC(double degC_value):
//     _degC_value(degC_value)
// {
//     if (_degC_value < MinDegC || MaxDegC < _degC_value)
//     {
//         throw Error(__FILE__, __LINE__, "An invalid temperature value was entered");  // 無効な温度の値が入力されました
//     }
// }


// /***** class _hz *****/

// _hz::_hz(double hz_value):
//     _hz_value(hz_value)
// {
//     if (_hz_value < MinHz || MaxHz < _hz_value)
//     {
//         throw Error(__FILE__, __LINE__, "An invalid frequency value was entered");  // 無効な周波数の値が入力されました
//     }
// }


// /***** class _m *****/

// _m::_m(double m_value):
//     _m_value(m_value)
// {
//     if (_m_value < Min_m || Max_m < _m_value)
//     {
//         throw Error(__FILE__, __LINE__, "An invalid length value was entered");  // 無効な長さの値が入力されました
//     }
// }


// /***** class _km *****/

// _km::_km(double km_value):
//     _km_value(km_value)
// {
//     if (_km_value < Min_km || Max_km < _km_value)
//     {
//         throw Error(__FILE__, __LINE__, "An invalid length value was entered");  // 無効な長さの値が入力されました
//     }
// }


// /***** class _rad *****/

// _rad::_rad(double rad_value):
//     _rad_value(not_minus0(fmod(rad_value, 2.0*M_PI)+ (rad_value>=0.0 ? 0.0 : 2.0*M_PI)))
//     {
//         if (rad_value < -10.0 || 10.0 < rad_value)
//         {
//             // 警告 : これはラジアン単位の数として大きすぎます．度単位と間違った可能性があります．
//             print("<<Warning>> file : %s, line : %d\n%f is too large a number in radians. You may have mistaken the unit of degree.", __FILE__, __LINE__, rad_value);
//         }
//     }


// /***** class _deg *****/

// _deg::_deg(double deg_value):
//     _deg_value(not_minus0(fmod(deg_value, 360.0)+ (deg_value>=0.0 ? 0.0 : 360.0)))
//     {
//         if (-5.0 < deg_value && deg_value < 5.0 && deg_value != 0.0)
//         {
//             // 警告 : これは°単位の数として小さすぎます．ラジアン単位と間違った可能性があります．
//             print("<<Warning>> file : %s, line : %d\n%f is too small a number of degrees. You may have mistaken the unit for radians.", __FILE__, __LINE__, deg_value);
//         }
//     }


// }