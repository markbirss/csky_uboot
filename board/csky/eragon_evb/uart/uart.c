#include <asm/arch-eragon/uart.h>
#include "uart_internal.h"
#include <asm/arch-eragon/gpio.h>
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

/* This function is used to transmit character,in query mode or interrupt mode.
 * Parameters:
 *       uartid--a basepointer, could be one of UART0, UART1, UART2 or UART3.
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
