/**************************************************
 * SPI通信の出入力に関するコードです
 * このファイルは，spi.hppに名前だけ書かれている関数の中身です
 * 
 * このファイルでは，SPIのマスター側の通信に関するコードが定義されています．
**************************************************/

//! @file spi.cpp
//! @brief SPIでの入出力

#include "spi.hpp"

#include <algorithm>
#include <initializer_list>
#include <vector>

namespace sc
{

/***** class SPI::MISO *****/

SPI::MISO::MISO(int miso_gpio):
    Pin(miso_gpio)
{
    if (get_gpio() != all_of(EnableSPI0_MISO) && get_gpio() != all_of(EnableSPI1_MISO))
    {
throw Error(__FILE__, __LINE__, "An incorrect MISO pin number was entered");  // 正しくないMISOピンの番号が入力されました
    }
}

SPI::ID SPI::MISO::get_spi_id() const
{
    if (get_gpio() == any_of(EnableSPI0_MISO))
    {
return ID::spi_0;
    } else if (get_gpio() == any_of(EnableSPI1_MISO)) {
return ID::spi_1;
    } else {
throw Error(__FILE__, __LINE__, "An incorrect MISO pin number was entered");  // 正しくないMISOピンの番号が入力されました
    }
}

/***** class SPI::CSs *****/

bool SPI::CSs::no_use() const
{
    for (Pin pin : _cs_pins)
    {
        if (Pin::Status.at(pin.get_gpio()) != PinStatus::NoUse)
        {
return false;
        }
    }
    return true;
}

/***** class SPI::SCK *****/

SPI::SCK::SCK(int sck_gpio):
    Pin(sck_gpio)
{
    if (get_gpio() != all_of(EnableSPI0_SCK) && get_gpio() != all_of(EnableSPI1_SCK))
    {
throw Error(__FILE__, __LINE__, "An incorrect SCK pin number was entered");  // 正しくないSCKピンの番号が入力されました
    }
}

SPI::ID SPI::SCK::get_spi_id() const
{
    if (get_gpio() == any_of(EnableSPI0_SCK))
    {
return ID::spi_0;
    } else if (get_gpio() == any_of(EnableSPI1_SCK)) {
return ID::spi_1;
    } else {
throw Error(__FILE__, __LINE__, "An incorrect SCK pin number was entered");  // 正しくないSCKピンの番号が入力されました
    }
}

/***** class SPI::MOSI *****/

SPI::MOSI::MOSI(int miso_gpio):
    Pin(miso_gpio)
{
    if (get_gpio() != all_of(EnableSPI0_MOSI) && get_gpio() != all_of(EnableSPI1_MOSI))
    {
throw Error(__FILE__, __LINE__, "An incorrect MOSI pin number was entered");  // 正しくないMOSIピンの番号が入力されました
    }
}

SPI::ID SPI::MOSI::get_spi_id() const
{
    if (get_gpio() == any_of(EnableSPI0_MOSI))
    {
return ID::spi_0;
    } else if (get_gpio() == any_of(EnableSPI1_MOSI)) {
return ID::spi_1;
    } else {
throw Error(__FILE__, __LINE__, "An incorrect MOSI pin number was entered");  // 正しくないMOSIピンの番号が入力されました
    }
}

/***** class SPI::CS *****/

SPI::CS::CS(int cs_gpio):
    Pin(cs_gpio) {}

/***** class SPI::MemoryAddr *****/

SPI::MemoryAddr::MemoryAddr(unsigned int memory_addr):
    _memory_addr(static_cast<uint8_t>(memory_addr))
    {
        if (MaxMemoryAddr < memory_addr)
        {
throw Error(__FILE__, __LINE__, "An incorrect memory address was entered");  // 正しくないメモリアドレスが入力されました
        }
    }

SPI::MemoryAddr::operator uint8_t() const
{
    return _memory_addr;
}

/***** class SPI *****/

SPI::SPI(MISO miso, CSs cs, SCK sck, MOSI mosi):
    SPI(miso, cs, sck, mosi, 10'000_hz) {}

SPI::SPI(MISO miso, CSs cs, SCK sck, MOSI mosi, Freq freq):
    _miso(miso), _sck(sck), _mosi(mosi), _freq(freq), _spi_id(miso.get_spi_id())
{
    if (!(miso.get_spi_id() == sck.get_spi_id() && sck.get_spi_id() == mosi.get_spi_id()))
    {
throw Error(__FILE__, __LINE__, "An incorrect SPI pin number was entered");  // 正しくないSPIのピン番号が入力されました
    }

    if (Pin::Status.at(_miso.get_gpio()) == PinStatus::NoUse && Pin::Status.at(_sck.get_gpio()) == PinStatus::NoUse && Pin::Status.at(_mosi.get_gpio()) == PinStatus::NoUse && cs.no_use())
    {
        Pin::Status.at(_miso.get_gpio()) = PinStatus::SpiMiso;
        Pin::Status.at(_sck.get_gpio()) = PinStatus::SpiSck;
        Pin::Status.at(_mosi.get_gpio()) = PinStatus::SpiMosi;
        for (Pin cs_pin : static_cast<std::vector<Pin>>(cs))
        {
            Pin::Status.at(cs_pin.get_gpio()) = PinStatus::SpiCs;
        }
    } else {
throw Error(__FILE__, __LINE__, "This pin is already in use");  // このピンは既に使用されています
    }

    if (SPI::IsUse[_spi_id])
    {
throw Error(__FILE__, __LINE__, "SPI cannot be reinitialized");  // SPIを再度初期化することはできません
    } else {
        SPI::IsUse[_spi_id] = true;
    }

    ::spi_init((_spi_id ? spi1 : spi0), static_cast<double>(_freq));  // pico-SDKの関数  SPIを初期化する

    ::gpio_set_function(_miso.get_gpio(), GPIO_FUNC_SPI);  // pico-SDKの関数  ピンの機能をSPIモードにする
    ::gpio_set_function(_sck.get_gpio(), GPIO_FUNC_SPI);  // pico-SDKの関数  ピンの機能をSPIモードにする
    ::gpio_set_function(_mosi.get_gpio(), GPIO_FUNC_SPI);  // pico-SDKの関数  ピンの機能をSPIモードにする
    for (Pin cs_pin : static_cast<std::vector<Pin>>(cs))
    {
        _cs_pins.push_back(GPIO<Out>(cs_pin));  // CSピンをGPIO出力用のピンとしてセットアップ
    }
}

void SPI::write(Binary output_data, CS cs_pin) const
{
    spi_write_blocking((_spi_id ? spi1 : spi0), output_data, output_data.size());  // pico-SDKの関数  SPIで送信
}

Binary SPI::read(size_t size, CS cs_pin) const
{
    std::vector<uint8_t> input_data(size);
    size_t input_size = 0;  // 実際には何バイト受信したか
    input_size = ::spi_read_blocking((_spi_id ? spi1 : spi0), 0, input_data.data(), size);  // pico-SDKの関数  SPIで受信
    input_data.resize(input_size);
    return Binary(input_data);
}

void SPI::write_memory(Binary output_data, CS cs_pin, MemoryAddr memory_addr) const
{
    Binary corrected_data = memory_addr + output_data;
    ::spi_write_blocking((_spi_id ? spi1 : spi0), cs_pin, corrected_data, corrected_data.size(), false);  // pico-SDKの関数  SPIで送信
}

Binary SPI::read_memory(DataSize size, CS cs_pin, MemoryAddr memory_addr) const
{
    std::vector<uint8_t> input_data(size);
    DataSize input_size = 0;  // 実際には何バイト受信したか
    ::spi_write_blocking((_spi_id ? spi1 : spi0), cs_pin, &memory_addr, 1, true);  // まず，メモリアドレスを送信
    input_size = ::spi_read_blocking((_spi_id ? spi1 : spi0), cs_pin, input_data.data(), size, false);  // pico-SDKの関数  SPIで受信
    input_data.resize(input_size);
    return Binary(input_data);
}

}
