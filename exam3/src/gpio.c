/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <LPC11Uxx.h>

#include "gpio.h"



#define RESET_PORT        (1)
#define RESET_PIN         (19)
#define RESET_INT_CH      (0)
#define RESET_INT_MASK    (1 << RESET_INT_CH)

#define PIN_DAP_LED       (1<<21) // HID
#define PIN_MSD_LED       (1<<20)
#define PIN_CDC_LED       (1<<11) // UART

void gpio_init(void) {
    // enable clock for GPIO port 0
    LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 6);

    // configure GPIO-LED as output
    // DAP led (green)
    LPC_GPIO->DIR[0]  |= (PIN_DAP_LED);
    LPC_GPIO->CLR[0]  |= (PIN_DAP_LED);

    // MSD led (red)
    LPC_GPIO->DIR[0]  |= (PIN_MSD_LED);
    LPC_GPIO->CLR[0]  |= (PIN_MSD_LED);

    // Serial LED (blue)
      LPC_IOCON->TDI_PIO0_11 |= 0x01;
    LPC_GPIO->DIR[0]  |= (PIN_CDC_LED);
    LPC_GPIO->CLR[0]  |= (PIN_CDC_LED);

    // configure Button as input
    LPC_GPIO->DIR[RESET_PORT]  &= ~(1 << RESET_PIN);

    /* Enable AHB clock to the FlexInt, GroupedInt domain. */
    LPC_SYSCON->SYSAHBCLKCTRL |= ((1<<19) | (1<<23) | (1<<24));
}

void gpio_set_dap_led(uint8_t state) {
	// Since Ameba connect it as high voltage inverse
	// switch the state to : 1 -> clear, 0->set
    if (state) {
        LPC_GPIO->CLR[0] |= (PIN_DAP_LED);
    } else {
        LPC_GPIO->SET[0] |= (PIN_DAP_LED);
    }
}

void gpio_set_cdc_led(uint8_t state) {
	// Since Ameba connect it as high voltage inverse
	// switch the state to : 1 -> clear, 0->set
    if (state) {
        LPC_GPIO->CLR[0] |= (PIN_CDC_LED);
    } else {
        LPC_GPIO->SET[0] |= (PIN_CDC_LED);
    }
}

void gpio_set_msd_led(uint8_t state) {
	// Since Ameba connect it as high voltage inverse
	// switch the state to : 1 -> clear, 0->set
    if (state) {
        LPC_GPIO->CLR[0] |= (PIN_MSD_LED);
    } else {
        LPC_GPIO->SET[0] |= (PIN_MSD_LED);
    }
}

