
#include <stdint.h>
#include "cmsis_os.h"
#include "gpio.h"


// TODO :
//    usb - serial : debug


//
// flags declartion
//
#define NO_TIMEOUT  (0xFFFF)

#define FLAGS_MAIN_500MS           	(1 << 0)
#define FLAGS_MAIN_1S           	(1 << 1)
#define FLAGS_MAIN_2S           	(1 << 2)


//
// global variables
//
OS_TID main_task_id;


//
// Tasks
//

// priority

#define LOWEST_PRIORITY             (1)     /* Priority 0 is reserved for the RTX idle task */
#define HIGHEST_PRIORITY            (254)   /* Priority 255 is reserved by RTX */

#define MAIN_TASK_PRIORITY          (8)
#define TIMER_TASK_PRIORITY         (11)

// stack

#define MAIN_TASK_STACK     (200)
#define TIMER_TASK_STACK (128)

static U64 stk_main_task[MAIN_TASK_STACK/8];
static U64 stk_timer_task[TIMER_TASK_STACK/8];




// Timer Tasks
// Timer task, set flags every 30mS and 90mS
void timer_task(void) {
    uint8_t i = 0;
    os_itv_set(50); // 500ms

    while(1) {
        os_itv_wait();

        os_evt_set(FLAGS_MAIN_500MS, main_task_id);
        if (!(i&0x1))
            os_evt_set(FLAGS_MAIN_1S, main_task_id);
        if (!(i&0x3))
            os_evt_set(FLAGS_MAIN_2S, main_task_id);
        i++;
    }
}



// Main Task
void main_task(void) 
{
	uint8_t msd_led=0;
	uint8_t cdc_led=0;
	uint8_t dap_led=0;
	uint32_t flags;

	//
    main_task_id = os_tsk_self();


    // Start timer tasks
    os_tsk_create_user(timer_task, TIMER_TASK_PRIORITY, (void *)stk_timer_task, TIMER_TASK_STACK);

    while (1) {


        os_evt_wait_or(   FLAGS_MAIN_500MS  // 500ms
        		        | FLAGS_MAIN_1S     // 1s
						| FLAGS_MAIN_2S,    // 2s
                        NO_TIMEOUT);

        flags = (uint32_t)os_evt_get();

        if ( flags & FLAGS_MAIN_500MS ) {
        	gpio_set_msd_led(msd_led=!msd_led);
        }
        if ( flags & FLAGS_MAIN_1S ) {
        	gpio_set_dap_led(dap_led=!dap_led);
        }
        if ( flags & FLAGS_MAIN_2S ) {
        	gpio_set_cdc_led(cdc_led=!cdc_led);
        }
    }
}


int main(void)
{
	gpio_init();
	gpio_set_cdc_led(0);
	gpio_set_dap_led(0);
	gpio_set_msd_led(0);

	os_sys_init_user(main_task, MAIN_TASK_PRIORITY, stk_main_task, MAIN_TASK_STACK);

	for (;;);
}

