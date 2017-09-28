/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <asm/uart.h>
#include <asm/arch/uart.h>
#include <asm/arch/gpio.h>
#include <asm/arch/hardware.h>
#include <common.h>
#include <serial.h>

/****************************************************************************/

s32 uart_open( u32 uart_addrbase)
{
    volatile UART_REG *info = (UART_REG *)uart_addrbase;
    s32 divisor;

    if (uart_addrbase == UART0_BASEADDR) {
        gpio_set_reuse(GPIOC, 0x3, GPIO_BEHARDWARE);
    } else if (uart_addrbase == UART2_BASEADDR) {
        gpio_set_reuse(GPIOB, 0x3, GPIO_BEHARDWARE);
    } else if (uart_addrbase == UART3_BASEADDR) {
        gpio_set_reuse(GPIOB, 0x30, GPIO_BEHARDWARE);
    }

    /*baudrate=(seriak clock freq)/(16*divisor).*/
    divisor = ((LSP_DEFAULT_FREQ / 115200) >> 4);
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
 s32 uart_close(u32 uart_addrbase)
{
    volatile UART_REG *info = (UART_REG *)uart_addrbase;

    info->uart_IER &= ~IER_RDA_INT_ENABLE;
    info->uart_IER &= !(IER_RDA_INT_ENABLE | IER_THRE_INT_ENABLE);
    return SUCCESS;
}

/* This function is used to transmit character,in query mode or interrupt mode.
 * Parameters:
 *       uartid--a basepointer, could be one of UART0, UART1, UART2 or UART3.
 *   brxquery--it indicates the receive mode: true - query mode, false -
 *   interrupt mode
 * Return: SUCCESS or FAILURE.
 */
s32 uart_put_char(u32 uart_addrbase, u8 ch)
{
    volatile UART_REG *info = (UART_REG *)uart_addrbase;

    while ((!(info->uart_LSR & LSR_TRANS_EMPTY)));
    info->uart_THR = ch;
    return SUCCESS;
}
