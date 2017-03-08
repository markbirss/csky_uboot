/*
 * board.c - uboot stage2 entrance.
 *
 * Copyright (C): 2010 Hangzhou C-SKY Microsystem Co.,LTD.
 * Author: Jing Qing  (qing_jing@c-sky.com)
 * Date: 2010-04-14
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <version.h>
#include <timestamp.h>
#include <net.h>
#include <serial.h>
#include <stdio_dev.h>
#include <linux/types.h>
#include <asm/arch-eragon/datatype.h>
#include <asm/arch-eragon/spiflash.h>
#include <asm/arch-eragon/gpio.h>
extern int cskymac_initialize(unsigned char dev_num, int base_addr);
extern void csky_exception_init(void);
extern long  _cskyboot_start;

extern int32_t uart_open( uint32_t uart_addrbase);
extern int32_t uart_put_char(uint32_t uart_addrbase, uint8_t ch);

extern int8_t get_boot_select(void);
//extern int init_ddr();
//extern uint32_t emmc_host_init(card_info_t *emmc_card_info);
extern void emmc_emmc_read(uint8_t slot_id, uint32_t from, uint32_t len, uint8_t *buf); 
extern void emmc_emmc_write(uint8_t slot_id, uint32_t to, uint32_t len, const uint8_t *buf);
#ifndef CONFIG_IDENT_STRING
#define CONFIG_IDENT_STRING ""
#endif

/************************************************************************
 * Init Utilities                                                       *
 ************************************************************************
 * Some of this code should be moved into the core functions,
 * or dropped completely,
 * but let's get it working (again) first...
 */

/*
 * this function is the entrance of stage2.Mainly realizes init Utilities 
 * then goto mainloop.
 */
#define SPIFLASH_BASE_VALUE     0x00
#define ERASE_ADDR              0x00
#define ERASE_LENGTH            0x10000
#define READ_ADDR               0x3000
#define UBOOT_SIZE              0x3400
#define WRITE_ADDR              0x00
#define WRITE_NUM               256
void board_init_f(ulong dummy)
{
    while(1);
}

