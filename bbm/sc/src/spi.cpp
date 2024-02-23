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

/***** class MISO *****/

MISO::MISO(int miso_gpio):
    Pin(miso_gpio)
{
    if (gpio() != all_of(EnableSPI0_MISO) && gpio() != all_of(EnableSPI1_MISO))
    {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect MISO pin number was entered"));  // 正しくないMISOピンの番号が入力されました
    }
}

SPI_ID MISO::get_spi_id() const
{
    if (gpio() == any_of(EnableSPI0_MISO))
    {
return SPI_ID::spi_0;
    } else if (gpio() == any_of(EnableSPI1_MISO)) {
return SPI_ID::spi_1;
    } else {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect MISO pin number was entered"));  // 正しくないMISOピンの番号が入力されました
    }
}

/***** class CS *****/

bool CS::no_use() const
{
    for (Pin pin : _cs_pins)
    {
        if (Pin::Status.at(pin.gpio()) != PinStatus::NoUse)
        {
return false;
        }
    }
    return true;
}

/***** class SCK *****/

SCK::SCK(int sck_gpio):
    Pin(sck_gpio)
{
    if (gpio() != all_of(EnableSPI0_SCK) && gpio() != all_of(EnableSPI1_SCK))
    {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect SCK pin number was entered"));  // 正しくないSCKピンの番号が入力されました
    }
}

SPI_ID SCK::get_spi_id() const
{
    if (gpio() == any_of(EnableSPI0_SCK))
    {
return SPI_ID::spi_0;
    } else if (gpio() == any_of(EnableSPI1_SCK)) {
return SPI_ID::spi_1;
    } else {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect SCK pin number was entered"));  // 正しくないSCKピンの番号が入力されました
    }
}

/***** class MOSI *****/

MOSI::MOSI(int miso_gpio):
    Pin(miso_gpio)
{
    if (gpio() != all_of(EnableSPI0_MOSI) && gpio() != all_of(EnableSPI1_MOSI))
    {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect MOSI pin number was entered"));  // 正しくないMOSIピンの番号が入力されました
    }
}

SPI_ID MOSI::get_spi_id() const
{
    if (gpio() == any_of(EnableSPI0_MOSI))
    {
return SPI_ID::spi_0;
    } else if (gpio() == any_of(EnableSPI1_MOSI)) {
return SPI_ID::spi_1;
    } else {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect MOSI pin number was entered"));  // 正しくないMOSIピンの番号が入力されました
    }
}

/***** class SPI::MemoryAddr *****/

SPI::MemoryAddr::MemoryAddr(unsigned int memory_addr):
    _memory_addr(static_cast<uint8_t>(memory_addr))
    {
        if (MaxMemoryAddr < memory_addr)
        {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect memory address was entered"));  // 正しくないメモリアドレスが入力されました
        }
    }

SPI::MemoryAddr::operator uint8_t() const
{
    return _memory_addr;
}

/***** class SPI *****/

SPI::SPI(MISO miso, CS cs, SCK sck, MOSI mosi):
    SPI(miso, cs, sck, mosi, 10000_hz)
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
}

SPI::SPI(MISO miso, CS cs, SCK sck, MOSI mosi, Frequency<Unit::Hz>  freq):
    _miso(miso), _sck(sck), _mosi(mosi), _freq(freq), _spi_id(miso.get_spi_id())
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (!(miso.get_spi_id() == sck.get_spi_id() && sck.get_spi_id() == mosi.get_spi_id()))
    {
throw std::invalid_argument(f_err(__FILE__, __LINE__, "An incorrect SPI pin number was entered"));  // 正しくないSPIのピン番号が入力されました
    } else if (Pin::Status.at(_miso.gpio()) != PinStatus::NoUse || Pin::Status.at(_sck.gpio()) != PinStatus::NoUse || Pin::Status.at(_mosi.gpio()) != PinStatus::NoUse || !cs.no_use()) {
throw std::logic_error(f_err(__FILE__, __LINE__, "This pin is already in use"));  // このピンは既に使用されています
    } else if (SPI::IsUse[_spi_id]) {
throw std::logic_error(f_err(__FILE__, __LINE__, "SPI cannot be reinitialized"));  // SPIを再度初期化することはできません
    }

    Pin::Status.at(_miso.gpio()) = PinStatus::SpiMiso;
    Pin::Status.at(_sck.gpio()) = PinStatus::SpiSck;
    Pin::Status.at(_mosi.gpio()) = PinStatus::SpiMosi;
    for (Pin cs_pin : cs.get())
    {
        Pin::Status.at(cs_pin.gpio()) = PinStatus::SpiCs;
    }

    SPI::IsUse[_spi_id] = true;

    ::spi_init((_spi_id ? spi1 : spi0), static_cast<double>(_freq));  // pico-SDKの関数  SPIを初期化する

    ::gpio_set_function(_miso.gpio(), GPIO_FUNC_SPI);  // pico-SDKの関数  ピンの機能をSPIモードにする
    ::gpio_set_function(_sck.gpio(), GPIO_FUNC_SPI);  // pico-SDKの関数  ピンの機能をSPIモードにする
    ::gpio_set_function(_mosi.gpio(), GPIO_FUNC_SPI);  // pico-SDKの関数  ピンの機能をSPIモードにする
    for (Pin cs_pin : cs.get())
    {
        _cs_pins.push_back(GPIO<Out>(cs_pin));  // CSピンをGPIO出力用のピンとしてセットアップ
        _cs_pins.back().write(1);  // CSピンをオンに設定
    }
}

void SPI::write(Binary output_data, CS cs_pin) const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (cs_pin.size() > 1)
    {
throw std::logic_error(f_err(__FILE__, __LINE__, "Cannot communicate with multiple devices at the same time"));  // 複数のデバイスと同時に通信することはできません
    }
    _cs_pins.at(cs_pin.get().at(0)).write(0);  // 通信先のデバイスにつながるCSピンをオフにして，通信の開始を伝える
    int output_size = 0;  // 実際には何バイト送信したか
    output_size = spi_write_blocking((_spi_id ? spi1 : spi0), output_data, output_data.size());  // pico-SDKの関数  SPIで送信
    _cs_pins.at(cs_pin.get().at(0)).write(1);  // 通信先のデバイスにつながるCSピンをオンにして，通信の終了を伝える
    if (output_size < 0)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "Failed to transmit via SPI. CsPin:%hhx", cs_pin.get().at(0)));  // SPIによる送信に失敗しました
    }
}

Binary SPI::read(std::size_t size, CS cs_pin) const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (cs_pin.size() > 1)
    {
throw std::logic_error(f_err(__FILE__, __LINE__, "Cannot communicate with multiple devices at the same time"));  // 複数のデバイスと同時に通信することはできません
    }
    std::vector<uint8_t> input_data(size);
    int input_size = 0;  // 実際には何バイト受信したか
    _cs_pins.at(cs_pin.get().at(0)).write(0);  // 通信先のデバイスにつながるCSピンをオフにして，通信の開始を伝える
    input_size = ::spi_read_blocking((_spi_id ? spi1 : spi0), 0, input_data.data(), size);  // pico-SDKの関数  SPIで受信
    _cs_pins.at(cs_pin.get().at(0)).write(1);  // 通信先のデバイスにつながるCSピンをオンにして，通信の終了を伝える
    if (input_size < 0)
    {
        // input_size = 0;
throw std::runtime_error(f_err(__FILE__, __LINE__, "Failed to receive via SPI. CsPin:%hhx", cs_pin.get().at(0)));  // SPIによる受信に失敗しました
    }
    input_data.resize(input_size);
    return Binary(input_data);
}

void SPI::write_memory(Binary output_data, CS cs_pin, MemoryAddr memory_addr) const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (cs_pin.size() > 1)
    {
throw std::logic_error(f_err(__FILE__, __LINE__, "Cannot communicate with multiple devices at the same time"));  // 複数のデバイスと同時に通信することはできません
    }
    Binary corrected_data = uint8_t(memory_addr & 0b01111111) + output_data;
    int output_size = 0;  // 実際には何バイト送信したか
    _cs_pins.at(cs_pin.get().at(0)).write(0);  // 通信先のデバイスにつながるCSピンをオフにして，通信の開始を伝える
    output_size = ::spi_write_blocking((_spi_id ? spi1 : spi0), corrected_data, corrected_data.size());  // pico-SDKの関数  SPIで送信
    _cs_pins.at(cs_pin.get().at(0)).write(1);  // 通信先のデバイスにつながるCSピンをオンにして，通信の終了を伝える
    if (output_size < 0)
    {
throw std::runtime_error(f_err(__FILE__, __LINE__, "Failed to transmit via SPI. CsPin:%hhx, MemoryAddr:%hhx", cs_pin.get().at(0), memory_addr));  // SPIによる送信に失敗しました
    }
}

Binary SPI::read_memory(std::size_t size, CS cs_pin, MemoryAddr memory_addr) const
{
    #ifndef NODEBUG
        std::cout << "\t [ func " << __FILE__ << " : " << __LINE__ << " ] " << std::endl; 
    #endif
    if (cs_pin.size() > 1)
    {
throw std::logic_error(f_err(__FILE__, __LINE__, "Cannot communicate with multiple devices at the same time"));  // 複数のデバイスと同時に通信することはできません
    }
    std::vector<uint8_t> input_data(size);
    uint8_t write_memory_addr = memory_addr | 0b10000000;
    int output_size = 0;  // 実際には何バイト送信したか
    int input_size = 0;  // 実際には何バイト受信したか
    _cs_pins.at(cs_pin.get().at(0)).write(0);  // 通信先のデバイスにつながるCSピンをオフにして，通信の開始を伝える
    output_size = ::spi_write_blocking((_spi_id ? spi1 : spi0), &write_memory_addr, 1);  // まず，メモリアドレスを送信
    input_size = ::spi_read_blocking((_spi_id ? spi1 : spi0), 0, input_data.data(), size);  // pico-SDKの関数  SPIで受信
    _cs_pins.at(cs_pin.get().at(0)).write(1);  // 通信先のデバイスにつながるCSピンをオンにして，通信の終了を伝える
    if (input_size < 0 || output_size < 0)
    {
        // input_size = 0;
throw std::runtime_error(f_err(__FILE__, __LINE__, "Failed to receive via SPI. CsPin:%hhx, MemoryAddr:%hhx", cs_pin.get().at(0), memory_addr));  // SPIによる受信に失敗しました
    }
    input_data.resize(input_size);
    return Binary(input_data);
}

}
