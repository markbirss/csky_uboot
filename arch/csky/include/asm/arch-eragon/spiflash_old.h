/******************************************************************************
 * @file     spiflash.h
 * @brief    The Head File for the spiflash driver definitions
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

#ifndef __SPIFLASH_H__
#define __SPIFLASH_H__

#include "datatype.h"
#include <configs/eragon.h>

typedef enum ERASE_TYPE_e{
    ERASE_BY_SECTOR,
    ERASE_BY_BLOCK,
    ERASE_BY_CHIP
}ERASE_TYPE_t;

int32_t spiflash_init(void);

/*
 * read the spiflash id.
 * Parameters:
 *   id: the pointer to the id buffer.
 * return: SUCCESS or FAILURE.
 */
int32_t spiflash_read_id(uint8_t *id);
/*
 * read the spiflash status register.
 * Parameters:
 *   status: the pointer to the status buffer.
 * return: SUCCESS or FAILURE.
 */
int32_t spi_norflash_read_status_register(uint8_t *status);

/*
 * write the spiflash status register.
 * Parameters:
 *   status: the pointer to the status buffer.
 * return: SUCCESS or FAILURE.
 */
int32_t spi_norflash_write_status_register(uint8_t *status);

/*
 * read data from the spiflash.
 * Parameters:
 *   src_addr: the start address to be read.
 *   dst_buf: the pointer to the data buffer.
 *   len: the read data length.
 * return: SUCCESS or FAILURE.
 */
int32_t spiflash_read(uint8_t id, uint64_t offset, uint8_t *buf, uint32_t length, uint32_t * retlen);

/*
 * erase the chip.
 * Parameters:
 *   None.
 * return: SUCCESS or FAILURE.
 */
int32_t spi_norflash_erase_chip(uint8_t id);

/*
 * erase the spiflash by block.
 * Parameters:
 *   src_addr: the start address to be erased.
 * return: SUCCESS or FAILURE.
 */
int32_t spi_norflash_erase_block(uint8_t id, uint32_t offset, uint32_t length);

/*
 * erase the spiflash by sector.
 * Parameters:
 *   src_addr: the start address to be erased.
 * return: SUCCESS or FAILURE.
 */
int32_t spi_norflash_erase_sector(uint8_t id, uint32_t offset, uint32_t length);

int32_t spiflash_erase(uint8_t id, ERASE_TYPE_t type, uint32_t offset, uint32_t length);
/*
 * write data to the spiflash by page.
 * Parameters:
 *   dst_addr: the start address to be write.
 *   src_buf: the pointer to the data buffer.
 *   len: the write data length.
 * return: SUCCESS or FAILURE.
 */
int32_t spi_norflash_write_page(uint8_t id, uint32_t dst_addr, uint8_t *src_buf, uint32_t len);

int32_t spiflash_write(uint8_t id, uint32_t offset, const uint8_t *buf, uint32_t length, uint32_t *retlen);
/*
 * program data to the spiflash.
 * Parameters:
 *   dst_addr: the start address to be program.
 *   src_buf: the pointer to the data buffer.
 *   len: the program data length.
 * return: SUCCESS or FAILURE.
 */
//int spi_norflash_program(uint32_t dst_addr, uint8_t *src_buf, uint8_t len);
#endif







