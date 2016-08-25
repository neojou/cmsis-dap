/* 
 *  * Contains the entry point for the project, and defines the ISR vectors
 *    Copyright (C) 2013 Richard Meadows
 *    
 *    Permission is hereby granted, free of charge, to any person obtaining
 *    a copy of this software and associated documentation files (the
 *    "Software"), to deal in the Software without restriction, including
 *    without limitation the rights to use, copy, modify, merge, publish,
 *    distribute, sublicense, and/or sell copies of the Software, and to
 *    permit persons to whom the Software is furnished to do so, subject to
 *    the following conditions:
 *             
 *    The above copyright notice and this permission notice shall be
 *    included in all copies or substantial portions of the Software.
 *                
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *    LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *    */

/*
 *  * A function for each of the interrupt vectors is weakly defined with an alias
 *  * so it may be optionally overridden elsewhere in the project.
 *  */
 
#define alias(f) __attribute__ ((alias(#f)))
#define __weak __attribute__ ((weak))

/* Cortex-M# Core Interrupts */
__weak void Reset_Handler(void);
__weak void NMI_Handler(void) alias (Default_Handler);
__weak void HardFault_Handler(void);
__weak void MemManage_Handler(void);
__weak void BusFault_Handler(void);
__weak void UsageFault_Handler(void);

/* LPCxxxx Chip Interrupts */

/* This is defined in the linker script */
extern void __StackLimit(void);
extern void __valid_user_code_checksum();


typedef void (*vect_t)(void);

extern	void SVC_Handler (void);
extern	void PendSV_Handler (void);
extern	void SysTick_Handler (void);

/*
 *  * This array of interrupt vectors is decared in a special section so that the
 *   * linker script can position it at 0x00000000.
 *    */
__attribute__ ((section(".isr_vector")))
const void *isr_vectors[] = {
    /* Cortex-M3 Core Interrupts */
    &__StackLimit,	  // The end of the stack.
    Reset_Handler,	  // The Reset handler
    NMI_Handler,		  // The NMI handler
    HardFault_Handler,	  // The Hard Fault Handler
    MemManage_Handler, 	  // The MPU fault handler
    BusFault_Handler,     // The bus fault handler
    UsageFault_Handler,   // The usage fault handler
    __valid_user_code_checksum,  // LPC MCU Checksum <- *** NEW IN LPCXPRESSO 7.9.0
    0, 	
	0,						  // Reserved
	0,						  // Reserved
	(vect_t)SVC_Handler,	  // SVCall Handler
	0,						  // Reserved
	0,						  // Reserved
	(vect_t)PendSV_Handler,   // PendSV Handler
	(vect_t)SysTick_Handler,  // SysTick Handler
};

/* CRP */
__attribute__ ((section(".crp")))
unsigned int CRP_KEY = 0xFFFFFFFF;



/* These are defined in the linker script */
extern unsigned int __etext;
extern unsigned int __data_start__;
extern unsigned int __data_end__;
extern unsigned int __bss_start__;
extern unsigned int __bss_end__;

extern void app_start(void);

/* The entry point to our program */
__attribute__ ((naked))
void Reset_Handler (void) {

  /* Load constants / initial values */
  for (unsigned int *source = &__etext, *destination = &__data_start__;
       destination < &__data_end__; )
    *destination++ = *source++;

  /* Zero out bss data */
  for (unsigned int *destination = &__bss_start__;
       destination < &__bss_end__; )
    *destination++ = 0;

  app_start();
  
  /* Wait here forever so the chip doesn't go haywire */
  while (1);
}

/* Default handler for undefined interrupts */
void Default_Handler(void) {
  /* You shouldn't have got here! There's been an undefined
 *      interrupt triggered somewhere.. */

  while(1);
}

/* HardFault Handler */
void HardFault_Handler(void) {
  /* Drat! A HardFault */

  while(1);
}

void MemManage_Handler(void) {
  while(1);
}

void BusFault_Handler(void) {
  while(1);
}

void UsageFault_Handler(void) {
  while(1);
}

