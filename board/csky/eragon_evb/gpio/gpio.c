/******************************************************************************
 * @file     gpio.c
 * @brief    The File for the gpio driver
 * @version  V1.0
 * @date     23. Dec 2016
 ******************************************************************************/
#include "gpio_internal.h"
#include <asm/arch-eragon/gpio.h>
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

