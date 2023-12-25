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
#include <iostream>  // coutなど
#include <string>

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


//! @brief ミリ秒間 待機
//! @param ms 待機する時間(ms)
//! @note RPi pico以外のマイコンを使用するときは，この関数を書き換えてください
inline void sleep_ms(uint32_t ms)
{
    ::sleep_ms(ms);
}

//! @brief マイクロ秒間 待機
//! @param us 待機する時間(μs)
//! @note RPi pico以外のマイコンを使用するときは，この関数を書き換えてください
inline void sleep_us(uint32_t us)
{
    ::sleep_us(us);
}

//! @brief ピンのGPIO番号を保持するデータクラス
class Pin
{
    const uint8_t _pin_gpio;  //! 扱うピンのGPIO番号
public:
    //! @brief GPIO番号からPinを作成
    //! @param pin_gpio GPIO番号
    //! @note 整数型からPin型に自動で変換されます
    Pin(uint8_t pin_gpio);
    
    //! @brief Pinをuint8_t型に変換
    //! @return uint8_t型のGPIO番号
    //! @note Pin型から整数型に自動で変換されます
    operator uint8_t() const;

    static constexpr MinGpio = 0;   //! 最小のGPIOピン番号
    static constexpr MaxGpio = 28;  //! 最大のGPIOピン番号

    //! @brief ピン番号として正しい値であるかを判定する
    //! @param pin_gpio 正しいかを判定したいGPIO番号
    //! @return ピン番号として取り得る値であったらtrue
    static bool is_correct_pin(uint8_t pin_gpio);
};


//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す
//! @param value 0かもしれない数
//! @return 0ではない数
template<typename T> inline T not0(T value) {return (value ? value : 1);}

//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す．floatバージョン
//! @param value 0かもしれない数
template<> inline float not0(float value) {return (value ? value : 1e-10);}

//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す．doubleバージョン
//! @param value 0かもしれない数
//! @return 0ではない数
template<> inline double not0(double value) {return (value ? value : 1e-10);}

//! @brief ゼロ除算防止のため，0であるかを判定し0でない数を返す．long doubleバージョン
//! @param value 0かもしれない数
//! @return 0ではない数
template<> inline long double not0(long double value) {return (value ? value : 1e-10);}


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

#endif  // SC19_PICO_SC_SC_BASIC_HPP_