/******************************************************************************
 * @file     spi_internal.c
 * @brief    The Head File for the spi driver
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

#ifndef __SPI_INTERNAL_H__
#define __SPI_INTERNAL_H__

#include <asm/arch-eragon/datatype.h>
#include <asm/arch-eragon/spi.h>


/* some register bits macro  */
#define CK_SPI_ENABLE           0x01
#define CK_SPI_DISABLE          0x00
#define CK_SPI_TMOD_BIT8        0x0100
#define CK_SPI_TMOD_BIT9        0x0200
#define CK_SPI_POLARITY         0x80
#define CK_SPI_PHASE            0x40
#define CK_SPI_BUSY             0x01
#define CK_SPI_INT_DISABLE      0x00
//#define CK_SPI_FIFO_LV          0x4
////#define CK_SPI_TXFIFO_EMPTY     0x04
//#define CK_SPI_RXFIFO_NOT_EMPTY 0x08
//#define CK_SPI_FIFO_MAX_LV      0x10
//#define CK_SPI_RXFIFO_OVERFLOW  0x08
//#define CK_SPI_RXFIFO_FULL      0x10
//#define CK_SPI_TXFIFO_EMPTY     0x01
//
///* some infoermationgs of SPI for special MCU */
//#define CK_SPI_DEFAULT_BAUDR    10000000   /* 10M */
//#define CK_SPI_MAXID            0x1


/* register visit struct  */
typedef struct {
    CK_SREG   CTRLR0;
    CK_SREG   rev0;
    CK_SREG   CTRLR1;
    CK_SREG   rev1;
    CK_CREG   SPIENR;
    CK_CREG   rev2[7];
    CK_REG    SER;
    CK_SREG   BAUDR;
    CK_SREG   rev3;
    CK_REG    TXFTLR;
    CK_REG    RXFTLR;
    CK_REG    TXFLR;
    CK_REG    RXFLR;
    CK_CREG   SR;
    CK_CREG   rev4[3];
    CK_REG    IMR;
    CK_REG    ISR;
    CK_REG    RISR;
    CK_CREG   TXOICR;
    CK_CREG   rev5[3];
    CK_CREG   RXOICR;
    CK_CREG   rev6[3];
    CK_CREG   RXUICR;
    CK_CREG   rev7[3];
    CK_CREG   MSTICR;
    CK_CREG   rev8[3];
    CK_CREG   ICR;
    CK_CREG   rev9[3];
    CK_CREG   DMACR;
    CK_CREG   rev10[3];
    CK_CREG   DMATDLR;
    CK_CREG   rev11[3];
    CK_CREG   DMARDLR;
    CK_CREG   rev12[3];
    CK_REG    IDR;
    CK_REG    rev13;
    CK_SREG   DR;
    CK_SREG   rev14[17];
    CK_CREG   WR;
}ckstruct_spi_reg_t, *pckstruct_spi_reg_t;

#endif
