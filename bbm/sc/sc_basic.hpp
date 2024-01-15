#ifndef SC19_PICO_SC_SC_BASIC_HPP_
#define SC19_PICO_SC_SC_BASIC_HPP_

/**************************************************
 * SC19のプロジェクト全体にかかわるコードです
 * このファイルは，sc_basic.cppに書かれている関数の一覧です
 * 
 * このファイルでは，エラーや待機，ログ出力などに関するコードが宣言されています．
**************************************************/

//! @file sc_basic.hpp
//! @brief プログラム全体で共通の，基本的な機能

#include "pico/stdlib.h"

#define _USE_MATH_DEFINES  // 円周率などの定数を使用する  math.hを読み込む前に定義する必要がある (math.hはcmathやiostreamに含まれる)
#include <algorithm>
#include <cfloat>  // double型の最小値など
#include <iostream>  // coutなど
#include <string>

#include "unit.hpp"
#include "pin.hpp"

namespace sc
{

//! @brief エラーを記録し保持
//! エラーを記録します．例外としてこのクラスを投げることができます．
//! @note エラーを発生させる場合は次のようにしてください
//!     throw sc::Error(__FILE__, __LINE__, "error message");
class Error : public std::exception
{
    const std::string _message;  // エラーメッセージ
public:
    //! @brief エラーを記録し，標準エラー出力に出力します
    //! @param FILE __FILE__としてください (自動でファイル名に置き換わります)
    //! @param LINE __LINE__としてください (自動で行番号に置き換わります)
    //! @param message 出力したいエラーメッセージ (自動で改行)
    Error(const std::string& FILE, int LINE, const std::string& message) noexcept;

    //! @brief キャッチしたエラーを記録し，標準エラー出力に出力します
    //! @param FILE ＿FILE＿としてください (自動でファイル名に置き換わります)
    //! @param LINE ＿LINE＿としてください (自動で行番号に置き換わります)
    //! @param message 出力したいエラーメッセージ (自動で改行)
    //! @param e キャッチした例外
    Error(const std::string& FILE, int LINE, const std::string& message, const std::exception& e) noexcept;

    //! @brief エラーについての説明文を返します
    //! @return エラーの説明
    const char* what() const noexcept override;
};


//! @brief メッセージを出力する関数です．
//! @param message 出力する文字列
//! 出力時にSDカードにも記録する場合などは，この関数に追記してください
void print(const std::string& message) noexcept;

//! @brief printfの形式で出力
//! @param format フォーマット文字列
//! @param args フォーマット文字列に埋め込む値
template<typename... Args>
void print(const std::string& format, Args... args) noexcept
{
    try
    {
        const size_t formatted_chars_num = std::snprintf(nullptr, 0, format.c_str(), args...);  // フォーマット後の文字数を計算
        char formatted_chars[formatted_chars_num + 1];  // フォーマット済みの文字列を保存するための配列を作成
        std::snprintf(&formatted_chars[0], formatted_chars_num + 1, format.c_str(), args...);  // フォーマットを実行
        print(formatted_chars);  // メッセージを出力
    }
    catch(const std::exception& e) {Error(__FILE__, __LINE__, "Failed to save log", e);}  // ログの保存に失敗しました
    catch(...) {Error(__FILE__, __LINE__, "Failed to save log");}  // ログの保存に失敗しました
}
// この関数は以下の資料を参考にて作成しました
// https://pyopyopyo.hatenablog.com/entry/2019/02/08/102456


//! @brief 指定した時間 待機
//! @param time 待機する時間 (100_ms のように入力)
//! @note RPi pico以外のマイコンを使用するときは，この関数を書き換えてください
inline void sleep(_ms time)
{
    ::sleep_us(static_cast<double>(static_cast<_us>(time)));
}


//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す
//! @param value 0かもしれない数
//! @return 0ではない数
template<typename T> inline T not0(T value) {return (value ? value : 1);}

//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す．floatバージョン
//! @param value 0かもしれない数
template<> inline float not0(float value) {return (value ? value : FLT_TRUE_MIN);}

//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す．doubleバージョン
//! @param value 0かもしれない数
//! @return 0ではない数
template<> inline double not0(double value) {return (value ? value : DBL_TRUE_MIN);}

//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す．long doubleバージョン
//! @param value 0かもしれない数
//! @return 0ではない数
template<> inline long double not0(long double value) {return (value ? value : LDBL_TRUE_MIN);}


//! @brief コピーを禁止するための親クラス
class Noncopyable
{
protected:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};
// Noncopyableクラスは以下の資料を参考にして作成しました
// https://cpp.aquariuscode.com/uncopyable-mixin

}


template<typename T> class all_of;  // 配列の要素と比較
template<typename T> constexpr bool operator!= (T, all_of<T>);  // 配列の全ての要素と等しくないか

//! @brief 配列の要素と比較
template<typename T>
struct all_of
{
    //! @brief 比較するためのリスト
    const std::vector<T> _compare_list;
    friend constexpr bool operator!=<> (T, all_of<T>);

public:
    //! @brief { }の要素と比較
    //! @param init_list {1, 2, 3}などのデータ
    all_of(std::initializer_list<T> init_list):
        _compare_list(init_list) {}

    //! @brief 配列の参照の要素と比較
    //! @param array_ref 長さの情報を持った配列
    template<std::size_t Size>
    all_of(const T (&array_ref)[Size]):
        _compare_list(array_ref, array_ref + Size) {}

    //! @brief 配列の要素と比較
    //! @param array_ptr 配列
    //! @param size 配列の長さ
    all_of(const T* array_ptr, size_t size):
        _compare_list(array_ptr, array_ptr + size) {}

    //! @brief 複数の引数と比較
    template<typename... Params>
    all_of(T elem1, T elem2, Params... elems):
        _compare_list({elem1, elem2, elems...}) {}

    //! @brief 複数の引数と比較
    //! @param input_itr vectorやarrayなど
    template<class InputItr>
    all_of(InputItr input_itr):
        _compare_list(input_itr.begin(), input_itr.end()) {}
};

//! @brief all_of 内の全ての要素と等しくないか
template<typename T>
constexpr bool operator!= (T left, all_of<T> right_list)
{
    return std::all_of(right_list._compare_list.begin(), right_list._compare_list.end(), [left](T right_1){return left != right_1;});
}


template<typename T> class any_of;  // 配列のいずれかの要素が等しいか
template<typename T> constexpr bool operator== (T, any_of<T>);  // 配列のいずれかの要素が等しいか

//! @brief 配列の要素と比較
template<typename T>
struct any_of
{
    //! @brief 比較するためのリスト
    const std::vector<T> _compare_list;
    friend constexpr bool operator==<> (T, any_of<T>);

public:
    //! @brief { }の要素と比較
    //! @param init_list {1, 2, 3}などのデータ
    any_of(std::initializer_list<T> init_list):
        _compare_list(init_list) {}

    //! @brief 配列の参照の要素と比較
    //! @param array_ref 長さの情報を持った配列
    template<std::size_t Size>
    any_of(const T (&array_ref)[Size]):
        _compare_list(array_ref, array_ref + Size) {}

    //! @brief 配列の要素と比較
    //! @param array_ptr 配列
    //! @param size 配列の長さ
    any_of(const T* array_ptr, size_t size):
        _compare_list(array_ptr, array_ptr + size) {}

    //! @brief 複数の引数と比較
    template<typename... Params>
    any_of(T elem1, T elem2, Params... elems):
        _compare_list({elem1, elem2, elems...}) {}

    //! @brief 複数の引数と比較
    //! @param input_itr vectorやarrayなど
    template<class InputItr>
    any_of(InputItr input_itr):
        _compare_list(input_itr.begin(), input_itr.end()) {}
};

//! @brief any_of 内のいずれかの要素と等しいか
template<typename T>
constexpr bool operator== (T left, any_of<T> right_list)
{
    return std::any_of(right_list._compare_list.begin(), right_list._compare_list.end(), [left](T right_1){return left == right_1;});
}

#endif  // SC19_PICO_SC_SC_BASIC_HPP_