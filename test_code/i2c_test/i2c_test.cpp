#include "sc_pico.hpp"
#include "pico/i2c_slave.h"


static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 9600; // 100 kHz

// For this example, we run both the master and slave from the same board.
// You'll need to wire pin GP4 to GP6 (SDA), and pin GP5 to GP7 (SCL).
static const uint I2C_SLAVE_SDA_PIN = 2;
static const uint I2C_SLAVE_SCL_PIN = 3;

// The slave implements a 256 byte memory. To write a series of bytes, the master first
// writes the memory address, followed by the data. The address is automatically incremented
// for each byte transferred, looping back to 0 upon reaching the end. Reading is done
// sequentially from the current memory address.
static struct
{
    uint8_t mem[256];
    uint8_t mem_address;
    bool mem_address_written;
} context;

// Our handler is called from the I2C ISR, so it must complete quickly. Blocking calls /
// printing to stdio may interfere with interrupt handling.
static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    switch (event) {
    case I2C_SLAVE_RECEIVE: // master has written some data
        if (!context.mem_address_written) {
            // writes always start with the memory address
            context.mem_address = i2c_read_byte_raw(i2c);
            context.mem_address_written = true;
        } else {
            // save into memory
            context.mem[context.mem_address] = i2c_read_byte_raw(i2c);
            context.mem_address++;
        }
        break;
    case I2C_SLAVE_REQUEST: // master is requesting data
        // load from memory
        i2c_write_byte_raw(i2c, context.mem[context.mem_address]);
        context.mem_address++;
        break;
    case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
        context.mem_address_written = false;
        break;
    default:
        break;
    }
}

static void setup_slave() {
    gpio_init(I2C_SLAVE_SDA_PIN);
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SDA_PIN);

    gpio_init(I2C_SLAVE_SCL_PIN);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SCL_PIN);

    i2c_init(i2c1, I2C_BAUDRATE);
    // configure I2C0 for slave mode
    i2c_slave_init(i2c1, I2C_SLAVE_ADDRESS, &i2c_slave_handler);
}

int main()
{    

    try
    {
        stdio_init_all();

        sleep_ms(1000);

        setup_slave();

        pico::I2C i2c(pico::I2C::Pin(0, 1), 9600);

        uint8_t count = 0;

        while (true)
        {
            i2c.write(sc::Binary{0, ++count, ++count, ++count}, sc::I2C::SlaveAddr(I2C_SLAVE_ADDRESS));
            // while(true) sc::Log::write("start\n");
            sleep_ms(1000);
            // sc::Log::write("a\n");

            sc::Binary read_data = i2c.read_mem(3, sc::I2C::SlaveAddr(I2C_SLAVE_ADDRESS), sc::I2C::MemoryAddr(0));

            sc::Log::write("%u, %u, %u\n", read_data[0], read_data[1], read_data[2]);
            sleep_ms(1000);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }    

}