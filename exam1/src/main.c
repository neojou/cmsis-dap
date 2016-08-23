
#include <stdint.h>
#include "gpio.h"

void board_init(void)
{
}


int main(void)
{
    SystemInit();

    SystemCoreClockUpdate();

    board_init();

    gpio_init();

    gpio_set_dap_led(1);
    gpio_set_cdc_led(0);
    gpio_set_msd_led(0);

    while(1);
    return 0;
}

