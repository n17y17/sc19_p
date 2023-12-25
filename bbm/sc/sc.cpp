#include "sc.hpp"

/**************************************************
 * SC19のプロジェクト全体にかかわるコードです
 * このファイルは，sc.hppに名前だけ書かれている関数の中身です
 * 
 * このファイルでは，エラーや待機，ログ出力などに関するコードが定義されています．
**************************************************/

namespace sc
{

/***** class Error *****/

Error::Error(const std::string& FILE, int LINE, const std::string& message) noexcept:
    _message(message)
{
    try
    {
        const std::string output_message = "<<ERROR>>  FILE : " + std::string(FILE) + "  LINE : " + std::to_string(LINE) + "\n           MESSAGE : " + _message + "\n";  // 出力する形式に変形
        std::cerr << output_message << std::endl;  // cerrでエラーとして出力

        Log::write(output_message);  // エラーをログデータに記録
    }
    catch (const std::exception& e) {std::cerr << "<<ERROR>>  FILE : " << __FILE__ << "  LINE : " << __LINE__ << "/n           MESSAGE : Error logging failed.   " << e.what() << std::endl;}  // エラー：エラーログの記録に失敗しました
    catch(...) {std::cerr << "<<ERROR>>  FILE : " << __FILE__ << "  LINE : " << __LINE__ << "/n           MESSAGE : Error logging failed." << std::endl;}  // エラー：エラーログの記録に失敗しました
}

Error::Error(const std::string& FILE, int LINE, const std::string& message, const std::exception& e) noexcept:
    Error(FILE, LINE, message + "   " + e.what()) {}

const char* Error::what() const noexcept
{
    return _message.c_str();
}


/***** class Log *****/

void Log::write(const std::string& log) noexcept
{
    try
    {
        std::cout << log << std::flush;
    }
    catch(const std::exception& e) {Error(__FILE__, __LINE__, "Failed to save log", e);}  // ログの保存に失敗しました
    catch(...) {Error(__FILE__, __LINE__, "Failed to save log");}  // ログの保存に失敗しました
}


}