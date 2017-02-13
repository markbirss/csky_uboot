/******************************************************************************
 * @file     uart_internal.h
 * @brief    The Head File for the uart driver
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
#ifndef __UART_INTERNAL_H__
#define __UART_INTERNAL_H__

#include <linux/types.h>
#include <asm/arch-eragon/datatype.h>
#include <asm/arch-eragon/uart.h>


/* UART registers addr definition */
#define CK_UART_RBR            0x00    /* Receive Buffer Register (32 bits, R) */
#define CK_UART_THR            0x00    /* Transmit Holding Register (32 bits, W) */
#define CK_UART_DLL            0x00    /* Divisor Latch(Low)  (32 bits, R/W) */
#define CK_UART_IER            0x01    /* Interrupt Enable Register (32 bits, R/W) */
#define CK_UART_DLH            0x01    /* Divisor Latch(High) (32 bits, R/W) */
#define CK_UART_IIR            0x02    /* Interrupt Identity Register (32 bits, R) */
#define CK_UART_FCR            0x02    /* fifo Countrol Register (32 bits, W) */
#define CK_UART_LCR            0x03    /* Line Control Register (32 bits, R/W) */
#define CK_UART_MCR            0x04    /* Modem Control Register (32 bits, W) */
#define CK_UART_LSR            0x05    /* Line Status Register (32 bits, R) */
#define CK_UART_MSR            0x06    /* Modem Status Register (32 bits, R/W) */
#define CK_UART_USR            0x1f    /* UART Status Register (32 bits, R/W) */


#define UART_BUSY_TIMEOUT      1000000
#define UART_RECEIVE_TIMEOUT   1000
#define UART_TRANSMIT_TIMEOUT  1000


/* UART register bit definitions */

#define USR_UART_BUSY           0x01
#define LSR_DATA_READY          0x01
#define LSR_THR_EMPTY           0x20
#define IER_RDA_INT_ENABLE      0x01
#define IER_THRE_INT_ENABLE     0x02
#define IIR_NO_ISQ_PEND         0x01

#define LCR_SET_DLAB            0x80   /* enable r/w DLR to set the baud rate */
#define LCR_PARITY_ENABLE	0x08   /* parity enabled */
#define LCR_PARITY_EVEN         0x10   /* Even parity enabled */
#define LCR_PARITY_ODD          0xef   /* Odd parity enabled */
#define LCR_WORD_SIZE_5         0xfc   /* the data length is 5 bits */
#define LCR_WORD_SIZE_6         0x01   /* the data length is 6 bits */
#define LCR_WORD_SIZE_7         0x02   /* the data length is 7 bits */
#define LCR_WORD_SIZE_8         0x03   /* the data length is 8 bits */
#define LCR_STOP_BIT1           0xfb   /* 1 stop bit */
#define LCR_STOP_BIT2           0x04   /* 1.5 stop bit */

#define CK_LSR_PFE              0x80     
#define CK_LSR_TEMT             0x40
#define CK_LSR_THRE             0x40
#define	CK_LSR_BI               0x10
#define	CK_LSR_FE               0x08
#define	CK_LSR_PE               0x04
#define	CK_LSR_OE               0x02
#define	CK_LSR_DR               0x01
#define CK_LSR_TRANS_EMPTY      0x20


/*config the uart */

typedef struct CK_UART_Info_t {
  volatile uint32_t* addr;
} CKStruct_UartInfo, *PCKStruct_UartInfo;

/* Struct of Uart REG*/
typedef struct CK_UART_REG {
    union {
        CK_REG	uart_RBR;
        CK_REG	uart_THR;
        CK_REG	uart_DLL;
    };
    union {
        CK_REG	uart_IER;
        CK_REG	uart_DLH;
    };
    union {
        CK_REG	uart_IIR;
        CK_REG	uart_FCR;
    };
    CK_REG	uart_LCR;
    CK_REG	uart_MCR;
    CK_REG	uart_LSR;
    CK_REG	uart_MSR;
    CK_REG	uart_RSV[24];
    CK_REG	uart_USR;
}CK_UART_REG;

#endif
