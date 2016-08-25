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

#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_HAL_CM.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_MemBox.h"


/*----------------------------------------------------------------------------
 *      Functions
 *---------------------------------------------------------------------------*/

/*--------------------------- rt_init_stack ---------------------------------*/

void rt_init_stack (P_TCB p_TCB, FUNCP task_body) {
  /* Prepare TCB and saved context for a first time start of a task. */
  U32 *stk,i,size;

  /* Prepare a complete interrupt frame for first task start */
  size = p_TCB->priv_stack >> 2;
  if (size == 0) {
    size = (U16)os_stackinfo >> 2;
  }

  /* Write to the top of stack. */
  stk = &p_TCB->stack[size];

  /* Auto correct to 8-byte ARM stack alignment. */
  if ((U32)stk & 0x04) {
    stk--;
  }

  stk -= 16;

  /* Default xPSR and initial PC */
  stk[15] = INITIAL_xPSR;
  stk[14] = (U32)task_body;

  /* Clear R1-R12,LR registers. */
  for (i = 0; i < 14; i++) {
    stk[i] = 0;
  }

  /* Assign a void pointer to R0. */
  stk[8] = (U32)p_TCB->msg;

  /* Initial Task stack pointer. */
  p_TCB->tsk_stack = (U32)stk;

  /* Task entry point. */
  p_TCB->ptask = task_body;

  /* Set a magic word for checking of stack overflow. */
  p_TCB->stack[0] = MAGIC_WORD;
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

