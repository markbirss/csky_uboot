/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/arch-eragon/timer.h>
#include <asm/arch-eragon/datatype.h>
#include <linux/types.h>
#include "timer_internal.h"
#include <configs/eragon.h>

#define TIMER_LOAD_VALUE 0xff000000
#define TIMER_OVERRUN_TIME (TIMER_LOAD_VALUE/TIMER_DEFAULT_FREQ)*1000000
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
    *time = (TIMER_LOAD_VALUE - currentvalue)/(TIMER_DEFAULT_FREQ/1000000) + TIMER_OVERRUN_TIME * timerover_count;
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

