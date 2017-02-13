/******************************************************************************
 * @file     timer.c
 * @brief    The File for the timer driver
 * @version  V1.0
 * @date     23. Dec 2016
 ******************************************************************************/
/* ---------------------------------------------------------------------------
 * Copyright (C) 2016 CSKY Limited. All rights reserved.
 *
 * Redistribution and use of this software in source and binary forms,
 * with or without modification, are permitted provided that the following
 * conditions are met:
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   * Neither the name of CSKY Ltd. nor the names of CSKY's contributors may
 *     be used to endorse or promote products derived from this software without
 *     specific prior written permission of CSKY Ltd.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 * -------------------------------------------------------------------------- */
#include <asm/arch-eragon/timer.h>
#include <asm/arch-eragon/datatype.h>
#include <linux/types.h>
#include "timer_internal.h"
#include <configs/eragon.h>

#define TIMER_LOAD_VALUE 0xff000000
#define TIMER_OVERRUN_TIME (TIMER_LOAD_VALUE/APB_DEFAULT_FREQ)*1000000
uint32_t timerover_count = 0;
uint32_t lastvalue = 0xff000000;

int32_t timer_elapsed_time(enum_timer_device timerid, uint32_t *time)
{
    uint32_t currentvalue =0;
    pckstruct_timer_t ptimer;
    ptimer = (pckstruct_timer_t)timerid;
    currentvalue = ptimer->TxCurrentValue;
    if(lastvalue < currentvalue){
        timerover_count++;
    }
    *time = (TIMER_LOAD_VALUE - currentvalue)/(APB_DEFAULT_FREQ/1000000) + TIMER_OVERRUN_TIME * timerover_count;
    lastvalue = currentvalue;
    return SUCCESS;
}

/*
 * start the corresponding timer
 *
 * timerid: the mumber of the corresponding timer port;
 * timeout: the set time (uS)
 */
int32_t timer_start(enum_timer_device timerid)
{
   pckstruct_timer_t ptimer;

   ptimer = (pckstruct_timer_t)timerid;

   /*  load time(us)  */
   ptimer->TxLoadCount = TIMER_LOAD_VALUE;
   /*in user-defined running mode, if use design ware timer, you can dynamic cha     nge the user mode and free mode when you enable the timer*/
   ptimer->TxControl |= CK_TIMER_TXCONTROL_MODE;
   /* enable the corresponding timer */
   ptimer->TxControl |= CK_TIMER_TXCONTROL_ENABLE;
  /*in user-defined running mode*/
   return SUCCESS;
}
#if 0
void timer_set_count(enum_timer_device timerid, uint32_t count)
{
   pckstruct_timer_t ptimer;
   ptimer = (pckstruct_timer_t)timerid;
   /* if the timer does not open,return failure */
   ptimer->TxControl &= ~CK_TIMER_TXCONTROL_ENABLE;
   ptimer->TxLoadCount = count;
   ptimer->TxControl |= CK_TIMER_TXCONTROL_ENABLE;
}
#endif
/*
 * stop a designated timer
 *
 * timerid: the mumber of the corresponding timer port;
 * stop_val: the count value when the timer stops
 */
int32_t timer_stop(enum_timer_device timerid)
{
   uint32_t stop_val;
   pckstruct_timer_t ptimer;

   ptimer = (pckstruct_timer_t)timerid;
   /* disable the timer*/
   stop_val = ptimer->TxCurrentValue;
   ptimer->TxControl &= ~CK_TIMER_TXCONTROL_ENABLE;
   return stop_val;
}

#if 0
/*
 * read the current value of the timer
 *
 * timerid: the mumber of the corresponding timer;
 * current_val: the current count-value
 */
uint32_t timer_current_value(enum_timer_device timerid)
{
   pckstruct_timer_t ptimer;
   ptimer = (pckstruct_timer_t)timerid;
   return ptimer->TxCurrentValue;
}
#endif

