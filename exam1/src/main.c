
#include <stdint.h>
#include "gpio.h"

void board_init(void)
{
}


int main(void)
{
    board_init();

    gpio_init();

    gpio_set_dap_led(1);
    gpio_set_cdc_led(1);
    gpio_set_msd_led(1);

    while(1);
    return 0;
}

