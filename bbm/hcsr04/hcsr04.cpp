#include "hcsr04.hpp" //クラス定義

// prototype ...
void hcsr_callback(uint gpio, uint32_t emask);

// global variable ...
static absolute_time_t up_edge_time;
static absolute_time_t dn_edge_time;
static uint64_t dtime = 0;

HCSR04::HCSR04(){
    gpio_init(28);
    gpio_set_dir(28, GPIO_OUT);
    gpio_pull_down(28);

    gpio_init(19);
    gpio_set_dir(19, GPIO_IN);
    gpio_set_irq_enabled_with_callback(19, GPIO_IRQ_EDGE_RISE + GPIO_IRQ_EDGE_FALL, true, &hcsr_callback);
}

int HCSR04::gettingTime()
{
    // trigger
    gpio_put(28, 1);
    busy_wait_us(12);
    gpio_put(28, 0);

    // wait
    busy_wait_ms(100);
    
    return dtime;
}

// function: callback of hcsr04 echo signal intrrupt
void hcsr_callback(uint gpio, uint32_t emask) {
    // irq disable
    gpio_set_irq_enabled(gpio, (GPIO_IRQ_EDGE_RISE + GPIO_IRQ_EDGE_FALL), false);

    if (emask == GPIO_IRQ_EDGE_RISE) {
        // high -> up edge detect: count start
        up_edge_time = get_absolute_time();
    } else if (emask == GPIO_IRQ_EDGE_FALL) {
        // low -> down edge detect: calculate time from rise edge to fall edge
        dn_edge_time = get_absolute_time();
        dtime = absolute_time_diff_us(up_edge_time, dn_edge_time);
    } else {
        // do nothing
    }

    // irq enable
    gpio_set_irq_enabled(gpio, (GPIO_IRQ_EDGE_RISE + GPIO_IRQ_EDGE_FALL), true);
}