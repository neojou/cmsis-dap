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

#ifndef __RT_HAL_CM_H__
#define __RT_HAL_CM_H__



/* Definitions */
#define INITIAL_xPSR    0x01000000U
#define DEMCR_TRCENA    0x01000000U
#define ITM_ITMENA      0x00000001U
#define MAGIC_WORD      0xE25A2EA5U
#define MAGIC_PATTERN   0xCCCCCCCCU

#if ((__TARGET_ARCH_7_M || __TARGET_ARCH_7E_M) && !NO_EXCLUSIVE_ACCESS)
 #define __USE_EXCLUSIVE_ACCESS
#else
 #undef  __USE_EXCLUSIVE_ACCESS
#endif

#ifndef __CMSIS_GENERIC
#define __DMB() do {\
                   __schedule_barrier();\
                   __dmb(0xF);\
                   __schedule_barrier();\
                } while (0)
#endif

#undef  __USE_EXCLUSIVE_ACCESS

#if defined (__CORTEX_M0)
#ifndef __TARGET_ARCH_6S_M
#define __TARGET_ARCH_6S_M
#endif
#endif

#if defined (__VFP_FP__) && !defined(__SOFTFP__)
#define __TARGET_FPU_VFP
#endif

#define __inline inline
#define __weak   __attribute__((weak))

#ifndef __CMSIS_GENERIC

__attribute__((always_inline)) static inline void __enable_irq(void)
{
  __asm volatile ("cpsie i");
}

__attribute__((always_inline)) static inline U32 __disable_irq(void)
{
  U32 result;

  __asm volatile ("mrs %0, primask" : "=r" (result));
  __asm volatile ("cpsid i");
  return(result & 1);
}


#endif

/* NVIC registers */
#define NVIC_ST_CTRL    (*((volatile U32 *)0xE000E010U))
#define NVIC_ST_RELOAD  (*((volatile U32 *)0xE000E014U))
#define NVIC_ST_CURRENT (*((volatile U32 *)0xE000E018U))
#define NVIC_ISER         ((volatile U32 *)0xE000E100U)
#define NVIC_ICER         ((volatile U32 *)0xE000E180U)
#if defined(__TARGET_ARCH_6S_M)
#define NVIC_IP           ((volatile U32 *)0xE000E400U)
#else
#define NVIC_IP           ((volatile U8  *)0xE000E400U)
#endif
#define NVIC_INT_CTRL   (*((volatile U32 *)0xE000ED04U))
#define NVIC_AIR_CTRL   (*((volatile U32 *)0xE000ED0CU))
#define NVIC_SYS_PRI2   (*((volatile U32 *)0xE000ED1CU))
#define NVIC_SYS_PRI3   (*((volatile U32 *)0xE000ED20U))

#define OS_PEND_IRQ()   NVIC_INT_CTRL  = (1UL<<28)
#define OS_PENDING      ((NVIC_INT_CTRL >> 26) & 5U)
#define OS_UNPEND(fl)   NVIC_INT_CTRL  = (U32)(*fl = (U8)OS_PENDING) << 25
#define OS_PEND(fl,p)   NVIC_INT_CTRL  = (U32)(fl | (U8)(p<<2)) << 26
#define OS_LOCK()       NVIC_ST_CTRL   =  0x0005U
#define OS_UNLOCK()     NVIC_ST_CTRL   =  0x0007U

#define OS_X_PENDING    ((NVIC_INT_CTRL >> 28) & 1U)
#define OS_X_UNPEND(fl) NVIC_INT_CTRL  = (U32)(*fl = (U8)OS_X_PENDING) << 27
#define OS_X_PEND(fl,p) NVIC_INT_CTRL  = (U32)(fl | p) << 28
#if defined(__TARGET_ARCH_6S_M)
#define OS_X_INIT(n)    NVIC_IP[n>>2] |=  (U32)0xFFU << ((n & 0x03U) << 3); \
                        NVIC_ISER[n>>5] = (U32)1U << (n & 0x1FU)
#else
#define OS_X_INIT(n)    NVIC_IP[n] = 0xFFU; \
                        NVIC_ISER[n>>5] = (U32)1U << (n & 0x1FU)
#endif
#define OS_X_LOCK(n)    NVIC_ICER[n>>5] = (U32)1U << (n & 0x1FU)
#define OS_X_UNLOCK(n)  NVIC_ISER[n>>5] = (U32)1U << (n & 0x1FU)

/* Core Debug registers */
#define DEMCR           (*((volatile U32 *)0xE000EDFCU))

/* ITM registers */
#define ITM_CONTROL     (*((volatile U32 *)0xE0000E80U))
#define ITM_ENABLE      (*((volatile U32 *)0xE0000E00U))
#define ITM_PORT30_U32  (*((volatile U32 *)0xE0000078U))
#define ITM_PORT31_U32  (*((volatile U32 *)0xE000007CU))
#define ITM_PORT31_U16  (*((volatile U16 *)0xE000007CU))
#define ITM_PORT31_U8   (*((volatile U8  *)0xE000007CU))

/* Variables */
extern BIT dbg_msg;

/* Functions */
#ifdef __USE_EXCLUSIVE_ACCESS
 #define rt_inc(p)     while(__strex((__ldrex(p)+1U),p))
 #define rt_dec(p)     while(__strex((__ldrex(p)-1U),p))
#else
 #define rt_inc(p)     __disable_irq();(*p)++;__enable_irq();
 #define rt_dec(p)     __disable_irq();(*p)--;__enable_irq();
#endif

__inline U32 rt_inc_qi (U32 size, U8 *count, U8 *first) {
  U32 cnt,c2;
#ifdef __USE_EXCLUSIVE_ACCESS
  do {
    if ((cnt = __ldrex(count)) == size) {
      __clrex();
      return (cnt); }
  } while (__strex(cnt+1, count));
  do {
    c2 = (cnt = __ldrex(first)) + 1;
    if (c2 == size) c2 = 0;
  } while (__strex(c2, first));
#else
  __disable_irq();
  if ((cnt = *count) < size) {
    *count = cnt+1;
    c2 = (cnt = *first) + 1;
    if (c2 == size) c2 = 0;
    *first = c2;
  }
  __enable_irq ();
#endif
  return (cnt);
}

__inline void rt_systick_init (void) {
  NVIC_ST_RELOAD  = os_trv;
  NVIC_ST_CURRENT = 0;
  NVIC_ST_CTRL    = 0x0007;
  NVIC_SYS_PRI3  |= 0xFF000000;
}

__inline void rt_svc_init (void) {
  NVIC_SYS_PRI3 |= 0x00FF0000;
  NVIC_SYS_PRI2 |= (NVIC_SYS_PRI3<<(8+1)) & 0xFC000000;
}

extern void rt_init_stack (P_TCB p_TCB, FUNCP task_body);
extern void rt_set_PSP (U32 stack);
extern U32  rt_get_PSP (void);
extern void os_set_env (void);
extern void *_alloc_box (void *box_mem);
extern int  _free_box (void *box_mem, void *box);

extern void dbg_init (void);
extern void dbg_task_notify (P_TCB p_tcb, BOOL create);
extern void dbg_task_switch (U32 task_id);

#ifdef DBG_MSG
#define DBG_INIT() dbg_init()
#define DBG_TASK_NOTIFY(p_tcb,create) if (dbg_msg) dbg_task_notify(p_tcb,create)
#define DBG_TASK_SWITCH(task_id)      if (dbg_msg && (os_tsk.new!=os_tsk.run)) \
                                                   dbg_task_switch(task_id)
#else
#define DBG_INIT()
#define DBG_TASK_NOTIFY(p_tcb,create)
#define DBG_TASK_SWITCH(task_id)
#endif


#endif //__RT_HAL_CM_H__ //
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

