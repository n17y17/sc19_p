#include <stdio.h>

#include "pico/stdlib.h"
#include "sc_pico/sc_pico.hpp"

int main()
{
    pico::PinIO led(0, sc::PinIO::Direction::out);

    uint64_t count = 0;

    while (true)
    {
        led.write(1);
        sleep_ms((++count)%100);
        led.write(0);
        sleep_ms((++count)%100);
    }
}
