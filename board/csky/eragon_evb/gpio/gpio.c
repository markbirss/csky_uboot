/******************************************************************************
 * @file     gpio.c
 * @brief    The File for the gpio driver
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
#include "gpio_internal.h"
#include <asm/arch-eragon/gpio.h>
#include <configs/eragon.h>
/*
 * Choose the software mode or hardware mode for any IO bit.
 * Parameters:
 *   pins: choose the bits which you want to config.
 *   bSoftware:
 *       '1' -- the corresponding pins are software mode, or, as GPIO.
 *       '0' -- the corresponding pins are hardware mode, or, used as UART, LCDC *               etc.
 * return: SUCCESS or FAILURE.
 */
bool gpio_set_reuse(enum_gpio_device_t gpio_addrbase, uint32_t pins, enum_gpio_mode bhardware)
{
    pckstruct_gpio_t reg;

    reg = (pckstruct_gpio_t)gpio_addrbase;
    if(bhardware)
    {
        reg->PORT_CTL |= pins;
    }
    else
    {
        reg->PORT_CTL &= (~pins);
    }
    return SUCCESS;
}

