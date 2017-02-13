/******************************************************************************
 * @file     timer.h
 * @brief    The Head File for the timer driver definitions
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
#ifndef __TIMER_H__
#define __TIMER_H__

#include "datatype.h"
#include <linux/types.h>
#define CK_TIMER0_BASSADDR (uint32_t)(0x1fba2000)
#define CK_TIMER1_BASSADDR (uint32_t)(0x50000014)

/*
 * enum the timer
 */
typedef enum
{
    TIMER0 = CK_TIMER0_BASSADDR,
    TIMER1 = CK_TIMER1_BASSADDR,
} enum_timer_device;

/*
 * start the corresponding timer
 *
 * timerid: the mumber of the corresponding timer port;
 */
int32_t timer_start(enum_timer_device timerid);

/*
 * stop a designated timer
 *
 * timerid: the mumber of the corresponding timer port;
 */
int32_t timer_stop(enum_timer_device timerid);

/*
 * read the elapsed time
 *
 * timerid: the mumber of the corresponding timer;
 * time   : elapsed time, unit is microsecond(US)
 */
int32_t timer_elapsed_time(enum_timer_device timerid, uint32_t *time);

#endif
