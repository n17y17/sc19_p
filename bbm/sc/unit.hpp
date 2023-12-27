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

#include <math.h>

#ifndef M_PI  // もし，円周率(M_PI)が定義されていなかったら，円周率を定義する
    #define M_PI 3.14159265358979323846
#endif


namespace sc
{

//! @brief 単位の親クラス
class _unit
{
    //! @brief double型への変換を強制する仮想関数
    //! @note _Unit型を継承したクラスでは，以下のdouble型に変換する関数を定義しないとエラーになります．
    virtual operator double() const = 0;
};

//! @brief 無次元量
class _pure : _unit
{
    const double _pure_value;
public:
    //! @brief 無次元量のセット
    _pure(double pure_value):
        _pure_value(pure_value) {}
    
    //! @brief 無次元量をdouble型に変換
    operator double() const override {return _pure_value;}
};

//! @brief 光度(cd)
class _cd : _unit
{
    const double _cd_value;
public:
    //! @brief 光度(cd)のセット
    _cd(double cd_value):
        _cd_value(cd_value) {}
    
    //! @brief 光度(cd)をdouble型に変換
    operator double() const override {return _cd_value;}
};

//! @brief 温度(℃)
class _degC : _unit
{
    const double _degC_value;
public:
    //! @brief 温度(℃)のセット
    _degC(double degC_value):
        _degC_value(degC_value) {}
    
    //! @brief 温度(℃)をdouble型に変換
    operator double() const override {return _degC_value;}
};

//! @brief 周波数(Hz)
//! @note 正確な単位はHzですが，'_'の後に大文字を置くことができないためhzとしています
class _hz : _unit
{
    const double _hz_value;
public:
    //! @brief 周波数(Hz)のセット
    _hz(double hz_value):
        _hz_value(hz_value) {}
    
    //! @brief 周波数(Hz)をdouble型に変換
    operator double() const override {return _hz_value;}
};

class _km;

//! @brief 長さ(m)
class _m : _unit
{
    const double _m_value;
public:
    //! @brief 長さ(m)のセット
    _m(double m_value):
        _m_value(m_value) {}
    
    //! @brief 長さ(m)をdouble型に変換
    operator double() const override {return _m_value;}

    //! @brief 長さ(m)をkmに変換
    operator _km() const;
};

//! @brief 長さ(km)
class _km : _unit
{
    const double _km_value;
public:
    //! @brief 長さ(km)のセット
    _km(double km_value):
        _km_value(km_value) {}
    
    //! @brief 長さ(km)をdouble型に変換
    operator double() const override {return _km_value;}

    //! @brief 長さ(km)をmに変換
    operator _m() const;
};

_m::operator _km() const {return _km(_m_value / 1000.0);}
_km::operator _m() const {return _m(_km_value * 1000.0);}

//! @brief 加速度(m/s²)
class _m_s2 : _unit
{
    const double _m_s2_value;
public:
    //! @brief 加速度(m/s²)のセット
    _m_s2(double m_s2_value):
        _m_s2_value(m_s2_value) {}
    
    //! @brief 加速度(m/s²)をdouble型に変換
    operator double() const override {return _m_s2_value;}
};

class _hPa;

//! @brief 圧力(Pa)
//! @note 正確な単位はPaですが，'_'の後に大文字を置くことができないためpaとしています
class _pa : _unit
{
    const double _pa_value;
public:
    //! @brief 圧力(Pa)のセット
    _pa(double pa_value):
        _pa_value(pa_value) {}
    
    //! @brief 圧力(Pa)をdouble型に変換
    operator double() const override {return _pa_value;}

    //! @brief 圧力(Pa)をhPaに変換
    operator _hPa() const;
};

//! @brief 圧力(hPa)
class _hPa : _unit
{
    const double _hPa_value;
public:
    //! @brief 圧力(hPa)のセット
    _hPa(double hPa_value):
        _hPa_value(hPa_value) {}
    
    //! @brief 圧力(hPa)をdouble型に変換
    operator double() const override {return _hPa_value;}

    //! @brief 圧力(hPa)をPaに変換
    operator _pa() const;
};

_pa::operator _hPa() const {return _hPa(_pa_value / 100.0);}
_hPa::operator _pa() const {return _pa(_hPa_value * 100.0);}

//! @brief 画素数(px)
class _px : _unit
{
    const double _px_value;
public:
    //! @brief 画素数(px)のセット
    _px(double px_value):
        _px_value(px_value) {}
    
    //! @brief 画素数(px)をdouble型に変換
    operator double() const override {return _px_value;}
};

class _deg;

//! @brief 角度(rad)
class _rad : _unit
{
    const double _rad_value;
public:
    //! @brief 角度(rad)のセット
    _rad(double rad_value):
        _rad_value(rad_value) {}
    
    //! @brief 角度(rad)をdouble型に変換
    operator double() const override {return _rad_value;}

    //! @brief 角度(rad)をdegに変換
    operator _deg() const;
};

//! @brief 角度(°)
class _deg : _unit
{
    const double _deg_value;
public:
    //! @brief 角度(°)のセット
    _deg(double deg_value):
        _deg_value(deg_value) {}
    
    //! @brief 度分秒での角度(°)のセット
    _deg(double deg, double min, double s = 0.0):
        _deg_value(deg + (min / 60.0) + (s / 60.0 / 60.0)) {}
    
    //! @brief 角度(°)をdouble型に変換
    operator double() const override {return _deg_value;}

    //! @brief 角度(°)をradに変換
    operator _rad() const;
};

_rad::operator _deg() const {return _deg(_rad_value * 180.0 / M_PI);}
_deg::operator _rad() const {return _rad(_deg_value * M_PI / 180.0);}

class _deg_s;

//! @brief 角速度(rad/s)
class _rad_s : _unit
{
    const double _rad_s_value;
public:
    //! @brief 角速度(rad/s)のセット
    _rad_s(double rad_s_value):
        _rad_s_value(rad_s_value) {}
    
    //! @brief 角速度(rad/s)をdouble型に変換
    operator double() const override {return _rad_s_value;}

    //! @brief 角速度(rad/s)をdeg/sに変換
    operator _deg_s() const;
};

//! @brief 角速度(°/s)
class _deg_s : _unit
{
    const double _deg_s_value;
public:
    //! @brief 角速度(deg/s)のセット
    _deg_s(double deg_s_value):
        _deg_s_value(deg_s_value) {}
    
    //! @brief 角速度(deg/s)をdouble型に変換
    operator double() const override {return _deg_s_value;}

    //! @brief 角速度(deg/s)をrad/sに変換
    operator _rad_s() const;
};

_rad_s::operator _deg_s() const {return _deg_s(_rad_s_value * 180.0 / M_PI);}
_deg_s::operator _rad_s() const {return _rad_s(_deg_s_value * M_PI / 180.0);}

class _ms;
class _s;
class _min;

//! @brief マイクロ秒(μs)
class _us : _unit
{
    const double _us_value;
public:
    //! @brief マイクロ秒(μs)のセット
    _us(double us_value):
        _us_value(us_value) {}
    
    //! @brief マイクロ秒(μs)をdouble型に変換
    operator double() const override {return _us_value;}

    //! @brief マイクロ秒(μs)をミリ秒(ms)に変換
    operator _ms() const;
    //! @brief マイクロ秒(μs)を秒(s)に変換
    operator _s() const;
    //! @brief マイクロ秒(μs)を分(min)に変換
    operator _min() const;
};

//! @brief ミリ秒(ms)
class _ms : _unit
{
    const double _ms_value;
public:
    //! @brief ミリ秒(ms)のセット
    _ms(double ms_value):
        _ms_value(ms_value) {}
    
    //! @brief ミリ秒(ms)をdouble型に変換
    operator double() const override {return _ms_value;}

    //! @brief ミリ秒(ms)をマイクロ秒(μs)に変換
    operator _us() const;
    //! @brief ミリ秒(ms)を秒(s)に変換
    operator _s() const;
    //! @brief ミリ秒(ms)を分(min)に変換
    operator _min() const;
};

//! @brief 秒(s)
class _s : _unit
{
    const double _s_value;
public:
    //! @brief 秒(s)のセット
    _s(double s_value):
        _s_value(s_value) {}
    
    //! @brief 秒(s)をdouble型に変換
    operator double() const override {return _s_value;}

    //! @brief 秒(s)をマイクロ秒(μs)に変換
    operator _us() const;
    //! @brief 秒(s)をミリ秒(ms)に変換
    operator _ms() const;
    //! @brief 秒(s)を分(min)に変換
    operator _min() const;
};

//! @brief 分(min)
class _min : _unit
{
    const double _min_value;
public:
    //! @brief 分(min)のセット
    _min(double min_value):
        _min_value(min_value) {}
    
    //! @brief 分(min)をdouble型に変換
    operator double() const override {return _min_value;}

    //! @brief 分(min)をマイクロ秒(μs)に変換
    operator _us() const;
    //! @brief 分(min)をミリ秒(ms)に変換
    operator _ms() const;
    //! @brief 分(min)をミリ秒(ms)に変換
    operator _s() const;
};

_us::operator _ms() const {return _ms(_us_value / 1000.0);}
_ms::operator _s() const {return _s(_ms_value / 1000.0);}
_s::operator _min() const {return _min(_s_value / 1000.0);}

_min::operator _s() const {return _s(_min_value * 1000.0);}
_s::operator _ms() const {return _ms(_s_value * 1000.0);}
_ms::operator _us() const {return _us(_ms_value * 1000.0);}

_us::operator _s() const {return _s(_ms(_us_value));}
_us::operator _min() const {return _min(_s(_ms(_us_value)));}
_ms::operator _min() const {return _min(_s(_ms_value));}
_s::operator _us() const {return _us(_ms(_s_value));}
_min::operator _us() const {return _us(_ms(_s(_min_value)));}
_min::operator _ms() const {return _ms(_s(_min_value));}

//! @brief 磁束密度(mT)
class _mT : _unit
{
    const double _mT_value;
public:
    //! @brief 磁束密度(mT)のセット
    _mT(double mT_value):
        _mT_value(mT_value) {}
    
    //! @brief 磁束密度(mT)をdouble型に変換
    operator double() const override {return _mT_value;}
};

}

//! @brief 無次元量
sc::_pure operator"" _pure(const char* m_chars) {return sc::_pure(std::stod(m_chars, nullptr));}

//! @brief 光度(cd)
sc::_cd operator"" _cd(const char* cd_chars) {return sc::_cd(std::stod(cd_chars, nullptr));}

//! @brief 温度(℃)
sc::_degC operator"" _degC(const char* degC_chars) {return sc::_degC(std::stod(degC_chars, nullptr));}

//! @brief 周波数(Hz)
sc::_hz operator"" _hz(const char* hz_chars) {return sc::_hz(std::stod(hz_chars, nullptr));}

//! @brief 長さ(m)
sc::_m operator"" _m(const char* m_chars) {return sc::_m(std::stod(m_chars, nullptr));}

//! @brief 長さ(km)
sc::_km operator"" _km(const char* km_chars) {return sc::_km(std::stod(km_chars, nullptr));}

//! @brief 加速度(m/s²)
sc::_m_s2 operator"" _m_s2(const char* m_s2_chars) {return sc::_m_s2(std::stod(m_s2_chars, nullptr));}

//! @brief 圧力(Pa)
sc::_pa operator"" _pa(const char* pa_chars) {return sc::_pa(std::stod(pa_chars, nullptr));}

//! @brief 圧力(hPa)
sc::_hPa operator"" _hPa(const char* hPa_chars) {return sc::_hPa(std::stod(hPa_chars, nullptr));}

//! @brief 画素数(px)
sc::_px operator"" _px(const char* px_chars) {return sc::_px(std::stod(px_chars, nullptr));}

//! @brief 角度(rad)
sc::_rad operator"" _rad(const char* rad_chars) {return sc::_rad(std::stod(rad_chars, nullptr));}

//! @brief 角度(°)
sc::_deg operator"" _deg(const char* deg_chars) {return sc::_deg(std::stod(deg_chars, nullptr));}

//! @brief 角速度(rad/s)
sc::_rad_s operator"" _rad_s(const char* rad_s_chars) {return sc::_rad_s(std::stod(rad_s_chars, nullptr));}

//! @brief 角速度(°/s)
sc::_deg_s operator"" _deg_s(const char* deg_s_chars) {return sc::_deg_s(std::stod(deg_s_chars, nullptr));}

//! @brief マイクロ秒(μs)
sc::_us operator"" _us(const char* us_chars) {return sc::_us(std::stod(us_chars, nullptr));}

//! @brief ミリ秒(ms)
sc::_ms operator"" _ms(const char* ms_chars) {return sc::_ms(std::stod(ms_chars, nullptr));}

//! @brief 秒(s)
sc::_s operator"" _s(const char* s_chars) {return sc::_s(std::stod(s_chars, nullptr));}

//! @brief 分(min)
sc::_min operator"" _min(const char* min_chars) {return sc::_min(std::stod(min_chars, nullptr));}

//! @brief 磁束密度(mT)
sc::_mT operator"" _mT(const char* mT_chars) {return sc::_mT(std::stod(mT_chars, nullptr));}

#endif  // SC19_PICO_SC_UNIT_HPP_