/*----------------------------------------------------------------------------
 *      CMSIS-RTOS  -  RTX
 *----------------------------------------------------------------------------
 *      Name:    RT_TASK.H
 *      Purpose: Task functions and system start up.
 *      Rev.:    V4.79
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 1999-2009 KEIL, 2009-2015 ARM Germany GmbH
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


#ifndef __RT_TASK_H__
#define __RT_TASK_H__

/* Definitions */

/* Values for 'state'   */
#define INACTIVE        0U
#define READY           1U
#define RUNNING         2U
#define WAIT_DLY        3U
#define WAIT_ITV        4U
#define WAIT_OR         5U
#define WAIT_AND        6U
#define WAIT_SEM        7U
#define WAIT_MBX        8U
#define WAIT_MUT        9U

/* Return codes */
#define OS_R_TMO        0x01U
#define OS_R_EVT        0x02U
#define OS_R_SEM        0x03U
#define OS_R_MBX        0x04U
#define OS_R_MUT        0x05U

#define OS_R_OK         0x00U
#define OS_R_NOK        0xFFU

/* Variables */
extern struct OS_TSK os_tsk;
extern struct OS_TCB os_idle_TCB;

/* Functions */
extern void      rt_switch_req (P_TCB p_new);
extern void      rt_dispatch   (P_TCB next_TCB);
extern void      rt_block      (U16 timeout, U8 block_state);
extern void      rt_tsk_pass   (void);
extern OS_TID    rt_tsk_self   (void);
extern OS_RESULT rt_tsk_prio   (OS_TID task_id, U8 new_prio);
extern OS_TID    rt_tsk_create (FUNCP task, U32 prio_stksz, void *stk, void *argv);
extern OS_RESULT rt_tsk_delete (OS_TID task_id);
#ifdef __CMSIS_RTOS
extern void      rt_sys_init   (void);
extern void      rt_sys_start  (void);
#else
extern void      rt_sys_init   (FUNCP first_task, U32 prio_stksz, void *stk);
#endif


#endif // __RT_TASK_H__
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
