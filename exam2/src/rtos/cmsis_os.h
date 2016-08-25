/* ----------------------------------------------------------------------
 * Copyright (C) 2012 ARM Limited. All rights reserved.
 *
 * $Date:        5. June 2012
 * $Revision:    V1.01
 *
 * Project:      CMSIS-RTOS API
 * Title:        cmsis_os.h RTX header file
 *
 * Version 0.02
 *    Initial Proposal Phase
 * Version 0.03
 *    osKernelStart added, optional feature: main started as thread
 *    osSemaphores have standard behavior
 *    osTimerCreate does not start the timer, added osTimerStart
 *    osThreadPass is renamed to osThreadYield
 * Version 1.01
 *    Support for C++ interface
 *     - const attribute removed from the osXxxxDef_t typedef's
 *     - const attribute added to the osXxxxDef macros
 *    Added: osTimerDelete, osMutexDelete, osSemaphoreDelete
 *    Added: osKernelInitialize
 * -------------------------------------------------------------------- */

/**
\page cmsis_os_h Header File Template: cmsis_os.h

The file \b cmsis_os.h is a template header file for a CMSIS-RTOS compliant Real-Time Operating System (RTOS).
Each RTOS that is compliant with CMSIS-RTOS shall provide a specific \b cmsis_os.h header file that represents
its implementation.

The file cmsis_os.h contains:
 - CMSIS-RTOS API function definitions
 - struct definitions for parameters and return types
 - status and priority values used by CMSIS-RTOS API functions
 - macros for defining threads and other kernel objects


<b>Name conventions and header file modifications</b>

All definitions are prefixed with \b os to give an unique name space for CMSIS-RTOS functions.
Definitions that are prefixed \b os_ are not used in the application code but local to this header file.
All definitions and functions that belong to a module are grouped and have a common prefix, i.e. \b osThread.

Definitions that are marked with <b>CAN BE CHANGED</b> can be adapted towards the needs of the actual CMSIS-RTOS implementation.
These definitions can be specific to the underlying RTOS kernel.

Definitions that are marked with <b>MUST REMAIN UNCHANGED</b> cannot be altered. Otherwise the CMSIS-RTOS implementation is no longer
compliant to the standard. Note that some functions are optional and need not to be provided by every CMSIS-RTOS implementation.


<b>Function calls from interrupt service routines</b>

The following CMSIS-RTOS functions can be called from threads and interrupt service routines (ISR):
  - \ref osSignalSet
  - \ref osSemaphoreRelease
  - \ref osPoolAlloc, \ref osPoolCAlloc, \ref osPoolFree
  - \ref osMessagePut, \ref osMessageGet
  - \ref osMailAlloc, \ref osMailCAlloc, \ref osMailGet, \ref osMailPut, \ref osMailFree

Functions that cannot be called from an ISR are verifying the interrupt status and return in case that they are called
from an ISR context the status code \b osErrorISR. In some implementations this condition might be caught using the HARD FAULT vector.

Some CMSIS-RTOS implementations support CMSIS-RTOS function calls from multiple ISR at the same time.
If this is impossible, the CMSIS-RTOS rejects calls by nested ISR functions with the status code \b osErrorISRRecursive.


<b>Define and reference object definitions</b>

With <b>\#define osObjectsExternal</b> objects are defined as external symbols. This allows to create a consistent header file
that is used throughout a project as shown below:

<i>Header File</i>
\code
#include <cmsis_os.h>                                         // CMSIS RTOS header file

// Thread definition
extern void thread_sample (void const *argument);             // function prototype
osThreadDef (thread_sample, osPriorityBelowNormal, 1, 100);

// Pool definition
osPoolDef(MyPool, 10, long);
\endcode


This header file defines all objects when included in a C/C++ source file. When <b>\#define osObjectsExternal</b> is
present before the header file, the objects are defined as external symbols. A single consistent header file can therefore be
used throughout the whole project.

<i>Example</i>
\code
#include "osObjects.h"     // Definition of the CMSIS-RTOS objects
\endcode

\code
#define osObjectExternal   // Objects will be defined as external symbols
#include "osObjects.h"     // Reference to the CMSIS-RTOS objects
\endcode

*/

#ifndef _CMSIS_OS_H
#define _CMSIS_OS_H

/// \note MUST REMAIN UNCHANGED: \b osCMSIS identifies the CMSIS-RTOS API version.
#define osCMSIS           0x10002U     ///< CMSIS-RTOS API version (main [31:16] .sub [15:0])

/// \note CAN BE CHANGED: \b osCMSIS_KERNEL identifies the underlying RTOS kernel and version number.
#define osCMSIS_RTX     ((4<<16)|81)   ///< RTOS identification and version (main [31:16] .sub [15:0])

/// \note MUST REMAIN UNCHANGED: \b osKernelSystemId shall be consistent in every CMSIS-RTOS.
#define osKernelSystemId "RTX V4.81"   ///< RTOS identification string


#define CMSIS_OS_RTX

//NeoJou
#define TOOLCHAIN_GCC

#ifndef __thumb2__
#define __thumb2__
#endif



/// \note MUST REMAIN UNCHANGED: \b osFeature_xxx shall be consistent in every CMSIS-RTOS.
#define osFeature_MainThread   1       ///< main thread      1=main can be thread, 0=not available
#define osFeature_Pool         1       ///< Memory Pools:    1=available, 0=not available
#define osFeature_MailQ        1       ///< Mail Queues:     1=available, 0=not available
#define osFeature_MessageQ     1       ///< Message Queues:  1=available, 0=not available
#define osFeature_Signals      16      ///< maximum number of Signal Flags available per thread
#define osFeature_Semaphore    65535   ///< maximum count for \ref osSemaphoreCreate function
#define osFeature_Wait         0       ///< osWait function: 1=available, 0=not available
#define osFeature_SysTick      1       ///< osKernelSysTick functions available

#if defined (__CC_ARM)
#define os_InRegs __value_in_regs      // Compiler specific: force struct in registers
#else
#define os_InRegs
#endif

#if   defined(__CC_ARM)
#define __NO_RETURN __declspec(noreturn)
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define __NO_RETURN __attribute__((noreturn))
#elif defined(__GNUC__)
#define __NO_RETURN __attribute__((noreturn))
#elif defined(__ICCARM__)
#define __NO_RETURN __noreturn
#else
#define __NO_RETURN
#endif

#include <stdint.h>
#include <stddef.h>


/* RL-ARM version number. */
#define __RL_ARM_VER    471

#define __task          __attribute__((noreturn))
#define __used          __attribute__((used))

#ifndef NULL
 #ifdef __cplusplus
  #define NULL          0
 #else
  #define NULL          ((void *) 0)
 #endif
#endif

#ifndef EOF
 #define EOF            (-1)
#endif

#ifndef __size_t
 #define __size_t       1
 typedef unsigned int   size_t;
#endif

#include "rt_TypeDef.h"

#ifdef __BIG_ENDIAN
 #define U32_LE(v)      (U32)(__rev(v))
 #define U16_LE(v)      (U16)(__rev(v) >> 16)
 #define U32_BE(v)      (U32)(v)
 #define U16_BE(v)      (U16)(v)
#else
 #define U32_BE(v)      (U32)(__rev(v))
 #define U16_BE(v)      (U16)(__rev(v) >> 16)
 #define U32_LE(v)      (U32)(v)
 #define U16_LE(v)      (U16)(v)
#endif
#define ntohs(v)        U16_BE(v)
#define ntohl(v)        U32_BE(v)
#define htons(v)        ntohs(v)
#define htonl(v)        ntohl(v)

/*----------------------------------------------------------------------------
 *                             RTX Kernel API
 *---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"  {
#endif

/* Definition of Semaphore type */
typedef U32 OS_SEM[2];

/* Definition of Mailbox type */
#define os_mbx_declare(name,cnt)    U32 name [4 + cnt]
typedef U32 OS_MBX[];

/* Definition of Mutex type */
typedef U32 OS_MUT[3];

/* Task Identification number. */
typedef U32 OS_TID;

/* Function return of system calls returning an object identification */
typedef void *OS_ID;

/* Function return of system calls indicating an event or completion state */
typedef U32 OS_RESULT;

/* Return codes */
#define OS_R_TMO        0x01
#define OS_R_EVT        0x02
#define OS_R_SEM        0x03
#define OS_R_MBX        0x04
#define OS_R_MUT        0x05

#define OS_R_OK         0x00
#define OS_R_NOK        0xff

#define OS_TCB_SIZE     48
#define OS_TMR_SIZE     8

/* Error Codes */
#define OS_ERR_STK_OVF  1
#define OS_ERR_FIFO_OVF 2
#define OS_ERR_MBX_OVF  3


/*----------------------------------------------------------------------------
 *      Cortex-M  SVC
 *---------------------------------------------------------------------------*/

// ==== Enumeration, structures, defines ====

#define __NO_RETURN __attribute__((noreturn))

typedef uint32_t __attribute__((vector_size(8)))  ret64;
typedef uint32_t __attribute__((vector_size(16))) ret128;

#define RET_pointer    __r0
#define RET_int32_t    __r0
#define RET_uint32_t   __r0
#define RET_OS_RESULT  __r0

#define SVC_ArgN(n) \
  register int __r##n __asm("r"#n);

#define SVC_ArgR(n,t,a) \
  register t   __r##n __asm("r"#n) = a;

#define SVC_Arg0()                                                             \
  SVC_ArgN(0)                                                                  \
  SVC_ArgN(1)                                                                  \
  SVC_ArgN(2)                                                                  \
  SVC_ArgN(3)

#define SVC_Arg1(t1)                                                           \
  SVC_ArgR(0,t1,a1)                                                            \
  SVC_ArgN(1)                                                                  \
  SVC_ArgN(2)                                                                  \
  SVC_ArgN(3)

#define SVC_Arg2(t1,t2)                                                        \
  SVC_ArgR(0,t1,a1)                                                            \
  SVC_ArgR(1,t2,a2)                                                            \
  SVC_ArgN(2)                                                                  \
  SVC_ArgN(3)

#define SVC_Arg3(t1,t2,t3)                                                     \
  SVC_ArgR(0,t1,a1)                                                            \
  SVC_ArgR(1,t2,a2)                                                            \
  SVC_ArgR(2,t3,a3)                                                            \
  SVC_ArgN(3)

#define SVC_Arg4(t1,t2,t3,t4)                                                  \
  SVC_ArgR(0,t1,a1)                                                            \
  SVC_ArgR(1,t2,a2)                                                            \
  SVC_ArgR(2,t3,a3)                                                            \
  SVC_ArgR(3,t4,a4)

#if (defined (__CORTEX_M0))
#define SVC_Call(f)                                                            \
  __asm volatile                                                               \
  (                                                                            \
    "ldr r7,="#f"\n\t"                                                         \
    "mov r12,r7\n\t"                                                           \
    "svc 0"                                                                    \
    :               "=r" (__r0), "=r" (__r1), "=r" (__r2), "=r" (__r3)         \
    :                "r" (__r0),  "r" (__r1),  "r" (__r2),  "r" (__r3)         \
    : "r7", "r12", "lr", "cc"                                                  \
  );
#else
#define SVC_Call(f)                                                            \
  __asm volatile                                                               \
  (                                                                            \
    "ldr r12,="#f"\n\t"                                                        \
    "svc 0"                                                                    \
    :               "=r" (__r0), "=r" (__r1), "=r" (__r2), "=r" (__r3)         \
    :                "r" (__r0),  "r" (__r1),  "r" (__r2),  "r" (__r3)         \
    : "r12", "lr", "cc"                                                        \
  );
#endif

#define SVC_0_1(f,t,rv)                                                        \
__attribute__((always_inline))                                                 \
static inline  t __##f (void) {                                                \
  SVC_Arg0();                                                                  \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_1_0(f,t,t1)                                                        \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1) {                                               \
  SVC_Arg1(t1);                                                                \
  SVC_Call(f);                                                                 \
}

#define SVC_1_1(f,t,t1,rv)                                                     \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1) {                                               \
  SVC_Arg1(t1);                                                                \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_2_1(f,t,t1,t2,rv)                                                  \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1, t2 a2) {                                        \
  SVC_Arg2(t1,t2);                                                             \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_3_0(f,t,t1,t2,t3)                                               \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1, t2 a2, t3 a3) {                                 \
  SVC_Arg3(t1,t2,t3);                                                          \
  SVC_Call(f);                                                                 \
}

#define SVC_3_1(f,t,t1,t2,t3,rv)                                               \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1, t2 a2, t3 a3) {                                 \
  SVC_Arg3(t1,t2,t3);                                                          \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_4_1(f,t,t1,t2,t3,t4,rv)                                            \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1, t2 a2, t3 a3, t4 a4) {                          \
  SVC_Arg4(t1,t2,t3,t4);                                                       \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_1_2 SVC_1_1 
#define SVC_1_3 SVC_1_1 
#define SVC_2_3 SVC_2_1 


/*----------------------------------------------------------------------------
 *      Functions Cortex-M
 *---------------------------------------------------------------------------*/


/* Task Management */
extern void      os_set_env    (void);
//extern void      rt_sys_init   (void (*task)(void), U8 priority, void *stk);
//extern void      rt_tsk_pass   (void);
//extern OS_TID    rt_tsk_self   (void);
//extern OS_RESULT rt_tsk_prio   (OS_TID task_id, U8 new_prio);
//extern OS_TID    rt_tsk_create (void (*task)(void), U8 priority, void *stk, void *argv);
//extern OS_RESULT rt_tsk_delete (OS_TID task_id);

#include "rt_task.h"

typedef void               (*FUNCP)(void);

SVC_3_0(rt_sys_init, void, FUNCP, U32, void*);
#if 0
extern void      _os_sys_init(U32 p, void (*task)(void), U32 prio_stksz,
                                     void *stk)                        __SVC_0;
extern OS_TID    _os_tsk_create (U32 p, void (*task)(void), U32 prio_stksz,
                                        void *stk, void *argv)         __SVC_0;
extern OS_TID    _os_tsk_create_ex (U32 p, void (*task)(void *), U32 prio_stksz,
                                           void *stk, void *argv)      __SVC_0;
extern OS_TID    _os_tsk_self (U32 p)                                  __SVC_0;
extern void      _os_tsk_pass (U32 p)                                  __SVC_0;
extern OS_RESULT _os_tsk_prio (U32 p, OS_TID task_id, U8 new_prio)     __SVC_0;
extern OS_RESULT _os_tsk_delete (U32 p, OS_TID task_id)                __SVC_0;
#endif


#define os_sys_init(tsk)              os_set_env();                           \
                                      __rt_sys_init(tsk,0,NULL)

#define os_sys_init_user(tsk,prio,stk,size)                                   \
                                      os_set_env();                           \
                                      __rt_sys_init(tsk,prio|(size<<8),stk)
                                      
#define os_sys_init_prio(tsk,prio)    os_set_env();                           \
                                      __rt_sys_init(tsk,prio,NULL)

#if 0
#define os_tsk_create(tsk,prio)       _os_tsk_create((U32)rt_tsk_create,tsk,prio,NULL,NULL)
#define os_tsk_create_user(tsk,prio,stk,size)                                 \
                                      _os_tsk_create((U32)rt_tsk_create,tsk,prio|(size<<8),stk,NULL)
#define os_tsk_create_ex(tsk,prio,argv) _os_tsk_create_ex((U32)rt_tsk_create,tsk,prio,NULL,argv)
#define os_tsk_create_user_ex(tsk,prio,stk,size,argv)                         \
                                      _os_tsk_create_ex((U32)rt_tsk_create,tsk,prio|(size<<8),stk,argv)
#define os_tsk_self()                 _os_tsk_self((U32)rt_tsk_self)
#define os_tsk_pass()                 _os_tsk_pass((U32)rt_tsk_pass)
#define os_tsk_prio(task_id,new_prio) _os_tsk_prio((U32)rt_tsk_prio,task_id,new_prio)
#define os_tsk_prio_self(prio)        _os_tsk_prio((U32)rt_tsk_prio,0,prio)
#define os_tsk_delete(task_id)        _os_tsk_delete((U32)rt_tsk_delete,task_id)
#define os_tsk_delete_self()          _os_tsk_delete((U32)rt_tsk_delete, 0)
#define isr_tsk_get()                 rt_tsk_self()
#endif


/* Time Management */
//extern U32       rt_time_get (void);
extern void      rt_dly_wait (U16 delay_time);
//extern void      rt_itv_set  (U16 interval_time);
//extern void      rt_itv_wait (void);



//#define os_time_get()              _os_time_get((U32)rt_time_get)

SVC_1_0(rt_dly_wait, void, U16);
#define os_dly_wait(delay_time)    __rt_dly_wait(delay_time);
//#define os_itv_set(interval_time)  _os_itv_set((U32)rt_itv_set,interval_time)
//#define os_itv_wait()              _os_itv_wait((U32)rt_itv_wait)

//extern U32       _os_time_get (U32 p)                                  __SVC_0;
//extern void      _os_dly_wait (U32 p, U16 delay_time)                  __SVC_0;
//extern void      _os_itv_set (U32 p, U16 interval_time)                __SVC_0;
//extern void      _os_itv_wait (U32 p)                                  __SVC_0;


#ifdef  __cplusplus
}
#endif

#endif  // _CMSIS_OS_H

