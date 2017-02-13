/******************************************************************************
 * @file     spi.c
 * @brief    The Head File for the spi interface
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

#ifndef __SPI_H__
#define __SPI_H__

#include "datatype.h"
#include <configs/eragon.h>

#define CK_SPI              CK_SPI0
typedef enum{
    CK_SPI0 = CK_SPI0_ADDRBASE,
    CK_SPI1 = CK_SPI1_ADDRBASE,
}enum_spi_device_t;

typedef enum{
    CK_SPI_TXRX = 0,
    CK_SPI_TX = 1,
    CK_SPI_RX = 2,
    CK_SPI_EERX=3
}enum_spi_mode_t;

typedef enum{
    CK_SPI_CLOCK_POLARITY_LOW = 0,
    CK_SPI_CLOCK_POLARITY_HIGH = 1
}enum_spi_polarity_t;

typedef enum{
    CK_SPI_CLOCK_PHASE_MIDDLE = 0,
    CK_SPI_CLOCK_PHASE_START = 1
}enum_spi_phase_t;

typedef enum{
    CK_SPI_DataSize_4 = 3,
    CK_SPI_DataSize_5 = 4,
    CK_SPI_DataSize_6 = 5,
    CK_SPI_DataSize_7 = 6,
    CK_SPI_DataSize_8 = 7,
    CK_SPI_DataSize_9 =  8,
    CK_SPI_DataSize_10 = 9,
    CK_SPI_DataSize_11 = 10,
    CK_SPI_DataSize_12 = 11,
    CK_SPI_DataSize_13 = 12,
    CK_SPI_DataSize_14 = 13,
    CK_SPI_DataSize_15 = 14,
    CK_SPI_DataSize_16 = 15
}enum_spi_datawidth_t;


typedef enum{
    CK_SPI_CS0 = 1,
    CK_SPI_CS1 = 2
}enum_spi_slave_t;


typedef enum{
    CK_SPI_TxComplete = 0,
    CK_SPI_RxComplete = 1,
    CK_SPI_Rx_Overflow = 2,
    CK_SPI_DMATransfer_Complete = 3,
    CK_SPI_DMATransfer_Error = 4
}enum_spi_error_t;


typedef enum{
    CK_SPI_DMA_RX,
    CL_SPI_DMA_TX
}enum_spi_direction_t;

/* definations of functions */

/*
 * disable the slave.
 * Parameters:
 *   id: the spi id.
 *   slave: choose the corresponding slave
 * return: SUCCESS or FAILURE.
 */
uint8_t spi_disable_slave(enum_spi_device_t id,enum_spi_slave_t slave);
/*
 * disable the spi.
 * Parameters:
 *   id: the spi id.
 * return: SUCCESS or FAILURE.
 */
int32_t spi_disable(enum_spi_device_t id);
/*
 * set the recieve num.
 * Parameters:
 *   id: the spi id.
 *   num: recieve data num.
 * return: SUCCESS or FAILURE.
 */
int32_t spi_set_ndf(enum_spi_device_t id,uint32_t num);
/*
 * enable the spi.
 * Parameters:
 *   id: the spi id.
 * return: SUCCESS or FAILURE.
 */
int32_t spi_enable(enum_spi_device_t id);


/*
 * Enable spi, and config the spi by default:
 *     baudrate -
 *     transfer mode - transmit&receive
 *     serial clock polarity - low
 *     serial clock phase - middle
 * Set the SPI irqhandler and register it into interrupt controller. Disable
 * the interrupts. Set the opening sign in CK_SPIInfo.
 * Parameters:
 *   id - SPI ID
 *   handler -
 *   bfast -
 * return: SUCCESS or FAILURE.
 */
int32_t spi_open(
    enum_spi_device_t spiid
    );

/*
 * Unable SPI, also, unregiste the SPI irq. Clear the opening sign.
 * Parameters:
 *   id - SPI ID
 * return: SUCCESS or FAILURE.
 *me: ckspi.h
 * Description: a head file for spi.c only. Include macros of register's bits,
 *              and others. Define manners to visit registers, and SPI info.
 * Author: Yun Ye (yun_ye@c-sky.com)
 * Date: Otc 13, 2008.
 */
int32_t spi_close(enum_spi_device_t spiid);

/*
 * Set the transfer mode of spi.
 * Parameters:
 *   id - SPI ID
 *   mode - has three modes: transmit&receive, transmit only, and receive only.
 * return: SUCCESS or FAILURE.
 */
int32_t spi_set_mode(
    enum_spi_device_t spiid,
    enum_spi_mode_t mode
    );

/*
 * Set the serial clock polarity.
 * Parameters:
 *   id - SPI ID
 *   polarity - serial clock polarity.
 * return: SUCCESS or FAILURE.
 */
int32_t spi_set_polarity(
    enum_spi_device_t spiid,
    enum_spi_polarity_t polarity
    );

/*
 * Set serial clock phase. When SCPH(CTRLR0[6])=0, data are captured on the
 * first edge of the serial clock. When SCPH=1, the serial clock starts
 * toggling one cycle after the slave select line is activated, and data
 * are capture one the second edge of the serial clock.
 * Parameters:
 *   id - SPI ID
 *   phase - serial clock phase
 * return: SUCCESS or FAILURE.
 */
int32_t spi_set_phase(
    enum_spi_device_t spiid,
    enum_spi_phase_t phase
    );

/*
 * Set the data frame size. The data size is determined by DFS(CTRLR0[3:0])
 * as below:
 *              DFS Value               Description
 *               0011                  4-bit serial data transfer
 *               0100                  5-bit serial data transfer
 *               ...                       ...
 *               1111                  16-bit serial data transfer
 * and the 0000 to 0010 are reserved.
 * Parameters:
 *   id - SPI ID
 *   datawidth - data frame size
 * return: SUCCESS or FAILURE.
 */
int32_t spi_set_datawidth(
    enum_spi_device_t spiid,
    enum_spi_datawidth_t datawidth
    );

/* Set the baudrate. The frequency of the sclk_out is derived from the
 * following equation:
 *   fsclk_out = fssi_clk / SCKDV
 * where SCKDV is any even value between 2 and 65534.
 * Parameters:
 *   id - SPI ID
 *   baudrate - transfer speed to be set
 * return: SUCCESS or FAILURE.
 */
int32_t spi_set_baudrate(
    enum_spi_device_t spiid,
    uint32_t baudrate
    );

/*
 * This function is used to transmit and receive datas in query mode.
 * Parameters:
 *   id - SPI ID
 *   slave - use to choose a slave peripheral
 *   ptx_buffer - memory head address used to store datas to be transmitted
 *   tx_length - data length to be transmitted
 *   prx_buffer - memory head address used to store datas to be received
 *   rx_length - length of data to be received
 *return:
 */
uint8_t spi_transfer(
    enum_spi_device_t spiid,
    enum_spi_slave_t slave,
    uint8_t *ptx_buffer,
    uint8_t tx_length,
    uint8_t *prx_buffer,
    uint32_t rx_length
    );

/*
 * When someone wants to transmit datas by DMA, he can use this function.
 * You just tell the transmit datas length and the address stores them, and the
 * function will do all the other things to complete the transfer.
 * Parameters:
 *    id - SPI ID
 *    slave - slave peripheral that you choose
 *    ptxbuffer - where  stores the datas
 *    itxlength - length of datas to be transmitted.
 * Return: NONE
 */
uint32_t spi_send_query(
       enum_spi_device_t spiid,
       enum_spi_slave_t slave,
       uint8_t *ptxbuffer,
       uint16_t txlength);

uint8_t spi_set_transfer_threshold(enum_spi_device_t spiid,uint8_t txftlr,uint8_t rxftlr);

/*
 * transfer the spi and slave by query mode.
 * Parameters:
 *   spiid: the spi id.
 *   slave: the slave num.
 *   ptx_buffer: the pointer to the send buffer.
 *   tx_length: the send data length.
 *   prx_buffer: the pointer to the recieve buffer.
 *   rx_length: the recieve data length.
 * return: SUCCESS or FAILURE.
 */
uint8_t spi_transfer_query(
    enum_spi_device_t spiid,
    enum_spi_slave_t slave,
    uint8_t *ptx_buffer,
    uint8_t tx_length,
    uint8_t *prx_buffer,
    uint32_t rx_length
    );
#endif







