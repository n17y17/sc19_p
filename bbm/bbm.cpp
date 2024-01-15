#include "bbm.hpp"

namespace sc
{

int main()
{
/***** setup *****/

    GPIO<In> us_pin(Pin(16)), ms_pin(17), s_pin(18), min_pin(19);
    print("start");
    // I2C isc(RX(5), SCL(2), 20'000_hz);
    GPIO<Out> led(Pin(2));
    LED led2(Pin(3));


/***** loop *****/
    while (true)
    {
        std::cout << "こんにちは" << std::endl;
        sleep(1_s);
        
    }

    return 0;
}

}

int main()
{
    stdio_init_all();
    sc::main();
}