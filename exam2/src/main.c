
#include <stdint.h>
#include "cmsis_os.h"
#include "gpio.h"

extern void main_task(void);


void main_task(void) 
{
	int i=0;

    while (1) {

        gpio_set_msd_led(((i&1)==0)?1:0);
        gpio_set_dap_led(((i&2)==0)?1:0);
        gpio_set_cdc_led(((i&4)==0)?1:0);

		i++;
        os_dly_wait(1000);
        
    }
}

#define MAIN_TASK_PRIORITY          (8)
#define MAIN_TASK_STACK     (200)
static U64 stk_main_task[MAIN_TASK_STACK/8];

int main(void)
{
	gpio_init();
	gpio_set_cdc_led(0);
	gpio_set_dap_led(0);
	gpio_set_msd_led(0);

	os_sys_init_user(main_task, MAIN_TASK_PRIORITY, stk_main_task, MAIN_TASK_STACK);

	for (;;);
}

