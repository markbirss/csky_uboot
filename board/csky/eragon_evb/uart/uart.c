/******************************************************************************
 * @file     uart.c
 * @brief    The File for the uart driver
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
#include <asm/arch-eragon/uart.h>
#include "uart_internal.h"
#include <asm/arch-eragon/gpio.h>
//#include <asm/arch-eragon/timer.h>
#include <configs/eragon.h>
#include <common.h>
#include <serial.h>

#define PCK_UART0_ADDRBASE  CK_UART0_ADDRBASE
#define PCK_UART1_ADDRBASE  CK_UART1_ADDRBASE
#define PCK_UART2_ADDRBASE  CK_UART2_ADDRBASE
#define PCK_UART3_ADDRBASE  CK_UART3_ADDRBASE
#define PCK_UART4_ADDRBASE  CK_UART4_ADDRBASE

/****************************************************************************/

int32_t uart_open( uint32_t uart_addrbase)
{
    volatile CK_UART_REG *info = (CK_UART_REG *)uart_addrbase;
    int32_t divisor;

    if (uart_addrbase == PCK_UART0_ADDRBASE) {
        gpio_set_reuse(GPIOC, 0x3, CK_GPIO_BEHARDWARE);
    } else if (uart_addrbase == PCK_UART2_ADDRBASE) {
        gpio_set_reuse(GPIOB, 0x3, CK_GPIO_BEHARDWARE);
    } else if (uart_addrbase == PCK_UART3_ADDRBASE) {
        gpio_set_reuse(GPIOB, 0x30, CK_GPIO_BEHARDWARE);
    }

    /*baudrate=(seriak clock freq)/(16*divisor).*/
    divisor = ((TIMER_DEFAULT_FREQ / 115200) >> 4);
    info->uart_LCR |= LCR_SET_DLAB;
    /* DLL and DLH is lower 8-bits and higher 8-bits of divisor.*/
    info->uart_DLL = divisor & 0xff;
    info->uart_DLH = (divisor >> 8) & 0xff;
    /*
     * The DLAB must be cleared after the baudrate is setted
     * to access other registers.
     */
    info->uart_LCR &= (~LCR_SET_DLAB);
    /* paraty */
    info->uart_FCR = 0x1;
    info->uart_LCR =  LCR_WORD_SIZE_8;
    info->uart_IER |= (IER_RDA_INT_ENABLE | IER_THRE_INT_ENABLE);

    return SUCCESS;
}

/* This function is used to close the uart
 * clear the callback function
 * free the irq
 * return: SUCCESS or FAILURE
 */
 int32_t uart_close(uint32_t uart_addrbase)
{
    volatile CK_UART_REG *info = (CK_UART_REG *)uart_addrbase;

    info->uart_IER &= ~IER_RDA_INT_ENABLE;
    info->uart_IER &= !(IER_RDA_INT_ENABLE | IER_THRE_INT_ENABLE);
    return SUCCESS;
}


/* This function is used to get character,in query mode or interrupt mode.
 * Parameters:
 * 	 uartid--a basepointer, could be one of UART0, UART1, UART2 or UART3.
 *   brxquery--it indicates the receive mode: true - query mode, false -
 *   interrupt mode
 * return: SUCCESS or FAILURE
 */
#if 0
int32_t uart_get_char(uint32_t uart_addrbase, uint8_t *ch)
{
    volatile CK_UART_REG *info = (CK_UART_REG *) uart_addrbase;
    uint32_t time;

    timer_start(TIMEOUT_COUNT_TIMER);
    while (!(info->uart_LSR & LSR_DATA_READY)) {
        if (timer_elapsed_time(TIMEOUT_COUNT_TIMER, &time) != SUCCESS ||
            time > TIMEOUT_VALUE * 1000) {
            /* time out */
            return FAILURE;
       }
    }
    *ch = info->uart_RBR;
    return SUCCESS;
}
#endif
/* This function is used to transmit character,in query mode or interrupt mode.
 * Parameters:
 * 	 uartid--a basepointer, could be one of UART0, UART1, UART2 or UART3.
 *   brxquery--it indicates the receive mode: true - query mode, false -
 *   interrupt mode
 * Return: SUCCESS or FAILURE.
 */
int32_t uart_put_char(uint32_t uart_addrbase, uint8_t ch)
{
    volatile CK_UART_REG *info = (CK_UART_REG *)uart_addrbase;

    while ((!(info->uart_LSR & CK_LSR_TRANS_EMPTY)));
    info->uart_THR = ch;
    return SUCCESS;
}
#if 0
void putc(const char ch)
{
     uart_put_char(CK_UART2_ADDRBASE, ch);
}

void puts(const char *str)
{
	while (*str)
		putc(*str++);
}
#endif
