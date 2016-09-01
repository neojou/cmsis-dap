
#include <stdint.h>
#include "cmsis_os.h"
#include "gpio.h"
#include "usb_device.h"


// TODO :
//    usb - serial : debug


//
// flags declartion
//
#define NO_TIMEOUT  (0xFFFF)

#define FLAGS_MAIN_30MS           	(1 << 0)
#define FLAGS_MAIN_60MS           	(1 << 1)
#define FLAGS_MAIN_90MS           	(1 << 2)


//
// global variables
//
OS_TID main_task_id;

// Global state of usb

typedef enum {
    USB_DISCONNECTED,
    USB_CONNECTING,
    USB_CONNECTED,
    USB_CHECK_CONNECTED,
    USB_CONFIGURED,
    USB_DISCONNECTING,
    USB_DISCONNECT_CONNECT
} USB_CONNECT;

USB_CONNECT usb_state;



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
    os_itv_set(3); // 30ms

    while(1) {
        os_itv_wait();

        os_evt_set(FLAGS_MAIN_30MS, main_task_id);
        if (!(i&0x1))
            os_evt_set(FLAGS_MAIN_60MS, main_task_id);
        if (!(i&0x3))
            os_evt_set(FLAGS_MAIN_90MS, main_task_id);
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

    //
    usbd_init();
    usb_state = USB_CONNECTING;

    while (1) {


        os_evt_wait_or(   FLAGS_MAIN_30MS  // 500ms
        		        | FLAGS_MAIN_60MS     // 1s
						| FLAGS_MAIN_90MS,    // 2s
                        NO_TIMEOUT);

        flags = (uint32_t)os_evt_get();

        if ( flags & FLAGS_MAIN_30MS ) {
        	//gpio_set_msd_led(msd_led=!msd_led);
        }
        if ( flags & FLAGS_MAIN_60MS ) {
        	//gpio_set_dap_led(dap_led=!dap_led);
        }
        if ( flags & FLAGS_MAIN_90MS ) {
        	gpio_set_cdc_led(cdc_led=!cdc_led);

        	// Update USB connect status
            switch (usb_state) {

                case USB_DISCONNECTING:
    				usbd_connect(0);
    				usb_state = USB_DISCONNECTED;
                    break;

                case USB_DISCONNECT_CONNECT:
    				usbd_connect(0);
    				usb_state = USB_CONNECTING;
                    break;

                case USB_CONNECTING:
    				usbd_connect(1);
    				usb_state = USB_CHECK_CONNECTED;
                    break;

                case USB_CHECK_CONNECTED:
                    //if(usbd_configured()) {
                    usb_state = USB_CONNECTED;
                    //}
                    break;

                case USB_CONNECTED:
                case USB_DISCONNECTED:
                default:
                    break;
            }

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

