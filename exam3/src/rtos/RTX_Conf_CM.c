/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    RTX_Conf_CM.C
 *      Purpose: Configuration of CMSIS RTX Kernel for Cortex-M
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 1999-2009 KEIL, 2009-2012 ARM Germany GmbH
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of ARM  nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/

#include "cmsis_os.h"
#include "RTX_Config.h"

#define __ASM					  __asm
#define __INLINE				  inline
#define __STATIC_INLINE 		  static inline
#define __NO_RETURN 			  __attribute__((noreturn))
#define __USED					  __attribute__((used))
#define __WEAK					  __attribute__((weak))

/*----------------------------------------------------------------------------
 *      RTX User configuration part BEGIN
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//
// <h>Thread Configuration
// =======================
//
//   <o>Number of concurrent running threads <0-250>
//   <i> Defines max. number of threads that will run at the same time.
//       counting "main", but not counting "osTimerThread"
//   <i> Default: 6
#ifndef OS_TASKCNT

#define OS_TASKCNT        6

#endif

//   <o>Default Thread stack size [bytes] <64-4096:8><#/4>
//   <i> Defines default stack size for threads with osThreadDef stacksz = 0
//   <i> Default: 200
#ifndef OS_STKSIZE
 #define OS_STKSIZE     110     // this stack size value is in words
#endif
//   <o>Main Thread stack size [bytes] <64-32768:8><#/4>
//   <i> Defines stack size for main thread.
//   <i> Default: 200
#ifndef OS_MAINSTKSIZE
 #define OS_MAINSTKSIZE 200      // this stack size value is in words
#endif
 
//   <o>Number of threads with user-provided stack size <0-250>
//   <i> Defines the number of threads with user-provided stack size.
//   <i> Default: 0
#ifndef OS_PRIVCNT
 #define OS_PRIVCNT    0
#endif
 
//   <o>Total stack size [bytes] for threads with user-provided stack size <0-1048576:8><#/4>
//   <i> Defines the combined stack size for threads with user-provided stack size.
//   <i> Default: 0
#ifndef OS_PRIVSTKSIZE
 #define OS_PRIVSTKSIZE 0       // this stack size value is in words
#endif

//   <o>Idle stack size [bytes] <64-4096:8><#/4>
//   <i> Defines default stack size for the Idle thread.
#ifndef OS_IDLESTKSIZE
#define OS_IDLESTKSIZE         64
#endif

//   <o>Timer Thread stack size [bytes] <64-4096:8><#/4>
//   <i> Defines stack size for Timer thread.
//   <i> Default: 200
#ifndef OS_TIMERSTKSZ
#define OS_TIMERSTKSZ  200
#endif

// <q>Check for stack overflow
// <i> Includes the stack checking code for stack overflow.
// <i> Note that additional code reduces the Kernel performance.
#ifndef OS_STKCHECK
#define OS_STKCHECK    0
#endif

//   <q>Stack usage watermark
//   <i> Initialize thread stack with watermark pattern for analyzing stack usage (current/maximum) in System and Thread Viewer.
//   <i> Enabling this option increases significantly the execution time of osThreadCreate.
#ifndef OS_STKINIT
#define OS_STKINIT     0
#endif
 
// <o>Processor mode for thread execution
//   <0=> Unprivileged mode
//   <1=> Privileged mode
// <i> Default: Privileged mode
#ifndef OS_RUNPRIV
#define OS_RUNPRIV     1
#endif

// </h>
// <h>SysTick Timer Configuration
// ==============================
//
//   <o>Timer clock value [Hz] <1-1000000000>
//   <i> Defines the timer clock value.
//   <i> Default: 6000000  (6MHz)
#ifndef OS_SYSTICK
 #define OS_SYSTICK     1
#endif

#ifndef OS_CLOCK

#define OS_CLOCK       (48000000UL)

#endif

//   <o>Timer tick value [us] <1-1000000>
//   <i> Defines the timer tick value.
//   <i> Default: 1000  (1ms)
#ifndef OS_TICK
#define OS_TICK        1000
#endif

// </h>

// <h>System Configuration
// =======================
//
// <e>Round-Robin Thread switching
// ===============================
//
// <i> Enables Round-Robin Thread switching.
#ifndef OS_ROBIN
#define OS_ROBIN       1
#endif

//   <o>Round-Robin Timeout [ticks] <1-1000>
//   <i> Defines how long a thread will execute before a thread switch.
//   <i> Default: 5
#ifndef OS_ROBINTOUT
#define OS_ROBINTOUT   5
#endif

// </e>

// <e>User Timers
// ==============
//   <i> Enables user Timers
#ifndef OS_TIMERS
#define OS_TIMERS      0
#endif

//   <o>Timer Thread Priority
//                        <1=> Low
//                        <2=> Below Normal
//                        <3=> Normal
//                        <4=> Above Normal
//                        <5=> High
//                        <6=> Realtime (highest)
//   <i> Defines priority for Timer Thread
//   <i> Default: High
#ifndef OS_TIMERPRIO
#define OS_TIMERPRIO   5
#endif

//   <o>Timer Thread stack size [bytes] <64-4096:8><#/4>
//   <i> Defines stack size for Timer thread.
//   <i> Default: 200
#ifndef OS_TIMERSTKSZ
 #define OS_TIMERSTKSZ  50     // this stack size value is in words
#endif
 
//   <o>Timer Callback Queue size <1-32>
//   <i> Number of concurrent active timer callback functions.
//   <i> Default: 4
#ifndef OS_TIMERCBQS
 #define OS_TIMERCBQS   4
#endif

// </e>

//   <o>ISR FIFO Queue size<4=>   4 entries  <8=>   8 entries
//                         <12=> 12 entries  <16=> 16 entries
//                         <24=> 24 entries  <32=> 32 entries
//                         <48=> 48 entries  <64=> 64 entries
//                         <96=> 96 entries
//   <i> ISR functions store requests to this buffer,
//   <i> when they are called from the interrupt handler.
//   <i> Default: 16 entries
#ifndef OS_FIFOSZ
#define OS_FIFOSZ      16
#endif

// </h>

//------------- <<< end of configuration section >>> -----------------------

// Standard library system mutexes
// ===============================
//  Define max. number system mutexes that are used to protect
//  the arm standard runtime library. For microlib they are not used.
#ifndef OS_MUTEXCNT
 #define OS_MUTEXCNT    8
#endif

/*----------------------------------------------------------------------------
 *      RTX User configuration part END
 *---------------------------------------------------------------------------*/

#define OS_TRV          ((uint32_t)((OS_CLOCK/1000)-1))
//#define OS_TRV          ((uint32_t)((OS_CLOCK*OS_TICK)-1)


#if (OS_SYSTICK == 0)   // Functions for alternative timer as RTX kernel timer
 
/*--------------------------- os_tick_init ----------------------------------*/
 
/// \brief Initializes an alternative hardware timer as RTX kernel timer
/// \return                             IRQ number of the alternative hardware timer
int os_tick_init (void) {
  return (-1);  /* Return IRQ number of timer (0..239) */
}
 
/*--------------------------- os_tick_val -----------------------------------*/
 
/// \brief Get alternative hardware timer's current value (0 .. OS_TRV)
/// \return                             Current value of the alternative hardware timer
uint32_t os_tick_val (void) {
  return (0);
}
 
/*--------------------------- os_tick_ovf -----------------------------------*/
 
/// \brief Get alternative hardware timer's  overflow flag
/// \return                             Overflow flag\n
///                                     - 1 : overflow
///                                     - 0 : no overflow
uint32_t os_tick_ovf (void) {
  return (0);
}
 
/*--------------------------- os_tick_irqack --------------------------------*/
 
/// \brief Acknowledge alternative hardware timer interrupt
void os_tick_irqack (void) {
  /* ... */
}
 
#endif   // (OS_SYSTICK == 0)
 

/*----------------------------------------------------------------------------
 *      RTX Errors
 *---------------------------------------------------------------------------*/

extern void mbed_die(void);

void mbed_die(void)
{
    for(;;);
}




/*----------------------------------------------------------------------------
 *      RTX Configuration Functions
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/

#if (OS_TASKCNT == 0)
#error "Invalid number of concurrent running threads!"
#endif

#if (OS_PRIVCNT >= OS_TASKCNT)
#error "Too many threads with user-provided stack size!"
#endif

#if (OS_TIMERS != 0)
#define OS_TASK_CNT (OS_TASKCNT + 1)
#define OS_PRIV_CNT (OS_PRIVCNT + 2)
#define OS_STACK_SZ (4*(OS_PRIVSTKSIZE+OS_MAINSTKSIZE+OS_TIMERSTKSZ))
#else
#define OS_TASK_CNT  OS_TASKCNT
#define OS_PRIV_CNT (OS_PRIVCNT + 1)
#define OS_STACK_SZ (4*(OS_PRIVSTKSIZE+OS_MAINSTKSIZE))
#endif

#ifndef OS_STKINIT
#define OS_STKINIT  0
#endif


uint16_t const os_maxtaskrun = OS_TASK_CNT;
uint32_t const os_stackinfo  = (OS_STKINIT<<28) | (OS_STKCHECK<<24) | (OS_PRIV_CNT<<16) | (OS_STKSIZE*4);
uint32_t const os_rrobin     = (OS_ROBIN << 16) | OS_ROBINTOUT;
uint32_t const os_tickfreq   = OS_CLOCK;
uint16_t const os_tickus_i   = OS_CLOCK/1000000;
uint16_t const os_tickus_f   = (((uint64_t)(OS_CLOCK-1000000*(OS_CLOCK/1000000)))<<16)/1000000;
uint32_t const os_trv        = OS_TRV;
uint8_t  const os_flags      = OS_RUNPRIV;

/* Export following defines to uVision debugger. */
extern uint32_t const CMSIS_RTOS_API_Version;
__USED uint32_t const CMSIS_RTOS_API_Version = osCMSIS;
extern uint32_t const CMSIS_RTOS_RTX_Version;
__USED uint32_t const CMSIS_RTOS_RTX_Version = osCMSIS_RTX;
__USED uint32_t const os_clockrate = OS_TICK;
__USED uint32_t const os_timernum  = 0U;

/* Memory pool for TCB allocation    */
extern
uint32_t       mp_tcb[];
_declare_box  (mp_tcb, OS_TCB_SIZE, OS_TASK_CNT);
uint16_t const mp_tcb_size;
uint16_t const mp_tcb_size = sizeof(mp_tcb);

/* Memory pool for System stack allocation (+os_idle_demon). */
uint64_t       mp_stk[];
_declare_box8 (mp_stk, OS_STKSIZE*4, OS_TASK_CNT-OS_PRIV_CNT+1);
uint32_t const mp_stk_size;
uint32_t const mp_stk_size = sizeof(mp_stk);

/* Memory pool for user specified stack allocation (+main, +timer) */
extern
uint64_t       os_stack_mem[];
uint64_t       os_stack_mem[2+OS_PRIV_CNT*(OS_STACK_SZ/8)];
extern
uint32_t const os_stack_sz;
uint32_t const os_stack_sz = sizeof(os_stack_mem);

#ifndef OS_FIFOSZ
#define OS_FIFOSZ       16
#endif

/* Fifo Queue buffer for ISR requests.*/
uint32_t       os_fifo[OS_FIFOSZ*2+1];
uint8_t  const os_fifo_size = OS_FIFOSZ;

/* An array of Active task pointers. */
extern
void *os_active_TCB[];
void *os_active_TCB[OS_TASK_CNT];


/* Legacy RTX User Timers not used */
uint32_t       os_tmr = 0U; 
uint32_t const *m_tmr = NULL;
uint16_t const mp_tmr_size = 0U;


/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/

/*--------------------------- os_idle_demon ---------------------------------*/

__task void os_idle_demon (void) {
  /* The idle demon is a system task, running when no other task is ready */
  /* to run. The 'os_xxx' function calls are not allowed from this task.  */

  for (;;) {
  /* HERE: include optional user code to be executed when no task runs.*/
  }
}


/*--------------------------- os_tmr_call -----------------------------------*/

void os_tmr_call (U16 info) {
  /* This function is called when the user timer has expired. Parameter   */
  /* 'info' holds the value, defined when the timer was created.          */

  /* HERE: include optional user code to be executed on timeout. */
}


/*--------------------------- os_error --------------------------------------*/

void os_error (U32 err_code) {
  /* This function is called when a runtime error is detected. Parameter */
  /* 'err_code' holds the runtime error code (defined in RTL.H).         */

  /* HERE: include optional code to be executed on runtime error. */
  for (;;);
}



/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

