#include <stdio.h>

#include "pico/stdlib.h"
#include "sc_pico.hpp"

int main()
{
    stdio_init_all();
    pico::UART uart_0(pico::UART::Pin(0, 1), 2400);
    pico::UART uart_1(pico::UART::Pin(4, 5), 2400);

    uint64_t count = 0;
    sc::Log::write("start");

    while (true)
    {
        try
        {
            // std::cout << "start" << std::endl;
            //     printf("start");
            // std::cout << std::string("start") << std::flush;
            ++count;
            uint8_t write_bits[8];
            for (int i = 0; i < 8; ++i)
            {
                write_bits[i] = ((count >> (8*i)) & 0xffu);
            }
            uart_0.write(sc::Binary(8, write_bits));
            sc::Log::write("write0 : %llu\n", count);
            // std::cout << count << std::endl;
            sleep_ms(100);
            sc::Binary read_bits(uart_1.read(8));
            uint64_t read_data = 0;
            for (uint i = 0; i < read_bits.size(); ++i)
            {
                read_data <<= 8;
                read_data += read_bits[i];
            }
            sc::Log::write("read1 : %llu\n", read_data);
            // std::cout << read_data << std::endl;
            sleep_ms(100);
        }
        catch(const std::exception& e)
        {
            // std::cerr << e.what() << '\n';
            sc::Error(__FILE__, __LINE__, "unknown error", e);
        }
        
        
    }
}