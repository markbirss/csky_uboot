/******************************************************************************
 * @file     uart.h
 * @brief    The Head File for the uart driver definitions
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
#ifndef __UART_H__
#define __UART_H__

#include "datatype.h"
#include <linux/types.h>
/************************************
 * (8 data bitbs, ODD, 1 stop bits)
 ***********************************/

typedef enum{
  B1200  =1200,
  B2400  =2400,
  B4800  =4800,
  B9600  =9600,
  B14400 =14400,
  B19200 =19200,
  B56000 =56000,
  B38400 =38400,
  B57600 =57600,
  B115200=115200
}UART_BAUDRATE;

#define BAUDRATE   19200


typedef enum{
  UART0 = 0,
  UART1 = 1,
  UART2 = 2,
  UART3 = 3
}UART_DEVICE;

typedef enum{
  WORD_SIZE_5 = 0,
  WORD_SIZE_6 = 1,
  WORD_SIZE_7 = 2,
  WORD_SIZE_8 = 3
}UART_WORDSIZE;

typedef enum{
  ODD = 0,
  EVEN = 1,
  NONE = 2
}UART_PARITY;

typedef enum{
  LCR_STOP_BIT_1 = 0,
  LCR_STOP_BIT_2 = 1
}UART_STOPBIT;


/////////////////////////////////////////////////////////////////
/* open the uart :
 * set the callback function --- handler(void);
 * intilize the serial port,sending and receiving buffer;
 * intilize irqhandler ;
 * register irqhandler
 * return: SUCCESS or FAILURE
 */
int32_t uart_open( uint32_t uart_addrbase);

/* This function is used to close the uart
 * clear the callback function
 * free the irq
 * return: SUCCESS or FAILURE
 */
 int32_t uart_close( uint32_t uart_addrbase);

/* This function is used to get character,in query mode or interrupt mode.
 * Parameters:
 * 	 uartid--a basepointer, could be one of UART0, UART1, UART2 or UART3.
 *   brxquery--it indicates the receive mode: TRUE - query mode, FALSE -
 *   interrupt mode
 * return: SUCCESS or FAILURE
 */
int32_t uart_get_char(uint32_t uart_addrbase,  uint8_t *ch);

/* This function is used to transmit character,in query mode or interrupt mode.
 * Parameters:
 * 	 uartid--a basepointer, could be one of UART0, UART1, UART2 or UART3.
 *   brxquery--it indicates the receive mode: TRUE - query mode, FALSE -
 *   interrupt mode
 * Return: SUCCESS or FAILURE.
 */
int32_t uart_put_char(uint32_t uart_addrbase, uint8_t ch);

#endif
