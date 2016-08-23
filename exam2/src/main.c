
#include <stdint.h>
#include "RTL.h"
#include "gpio.h"


#define MAIN_TASK_PRIORITY (10)
#define MAIN_TASK_STACK	   (800)
static uint64_t stk_main_task [MAIN_TASK_STACK / sizeof(uint64_t)];


void board_init(void)
{
}

void main_task(void)
{
    while (1) {
        gpio_set_cdc_led(0);
        gpio_set_dap_led(0);
        gpio_set_msd_led(0);

        os_dly_wait(1000);
        
        gpio_set_cdc_led(1);
        gpio_set_dap_led(1);
        gpio_set_msd_led(1);

        os_dly_wait(1000);

    }
}



int main(void)
{
    SystemInit();

    SystemCoreClockUpdate();

    board_init();

    gpio_init();


    os_sys_init_user(main_task, MAIN_TASK_PRIORITY, stk_main_task, MAIN_TASK_STACK);
    return 0;
}

