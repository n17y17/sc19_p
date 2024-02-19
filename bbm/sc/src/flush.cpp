/**************************************************
 * マイコンのフラッシュメモリに直接データを保存するためのコードです
 * このファイルは，flush.hppに名前だけ書かれている関数の中身です
 * 
**************************************************/

//! @file flush.cpp
//! @brief マイコンのフラッシュメモリに直接データを保存

#include "flush.hpp"

namespace sc
{

// Flush::Flush()
// {
//     // 割り込み無効にする
//     uint32_t ints = save_and_disable_interrupts();
//     // Flash消去。
//     //  消去単位はflash.hで定義されている FLASH_SECTOR_SIZE(4096Byte) の倍数とする
//     flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
//     // 割り込みフラグを戻す
//     restore_interrupts(ints);
// }


}