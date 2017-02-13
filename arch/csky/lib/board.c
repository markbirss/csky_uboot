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
#include <asm/arch-eragon/timer.h>
#include <asm/arch-eragon/spiflash.h>
#include <asm/arch-eragon/gpio.h>
extern int cskymac_initialize(unsigned char dev_num, int base_addr);
extern void csky_exception_init(void);
extern long  _cskyboot_start;

extern int32_t timer_start(enum_timer_device timerid);
extern int32_t uart_open( uint32_t uart_addrbase);
extern int32_t uart_put_char(uint32_t uart_addrbase, uint8_t ch);
extern int8_t get_boot_select(void);
//typedef int (init_fnc_t) (void);

extern int8_t get_boot_select(void);
//extern int init_ddr();
//extern uint32_t emmc_host_init(card_info_t *emmc_card_info);
extern void emmc_emmc_read(uint8_t slot_id, uint32_t from, uint32_t len, uint8_t *buf); 
extern void emmc_emmc_write(uint8_t slot_id, uint32_t to, uint32_t len, const uint8_t *buf);
#ifndef CONFIG_IDENT_STRING
#define CONFIG_IDENT_STRING	""
#endif

/************************************************************************
 * Init Utilities							*
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
#define READ_ADDR               0x2000
#define WRITE_ADDR              0x00
#define WRITE_NUM               256
void start_cskyboot (void)
{
    timer_start((uint32_t)0x1fba2000);
    uart_open((uint32_t)0x1fba4000);

#ifdef CONFIG_SPL_BUILD
    printf("The u-boot-spl start.\n");
//    int8_t om_judge;
    int i;
    uint8_t buff_w[512] = {0x0};
    uint8_t buff_r[512] = {0x0};
    uint32_t retlen;
    uint8_t *ddr_base = (uint8_t *)0x0;
    init_ddr();
    printf("ddr init ready.\n");
    emmc_host_init(NULL);
    printf("eMMC init ready.\n");
//    om_judge = get_boot_select();

//    switch(om_judge) {
//        case 0x0:
        /* The mode of serial */
        /* Not ready */
//        break;
//        case 0x1:
        /* The mode of spiflash */
#if 0
    spi_norflash_erase_sector(0, 0, 0x1000);
    spiflash_erase(0, ERASE_BY_SECTOR,ERASE_ADDR, ERASE_LENGTH);
    for(i = 0;i < 256;i += 4) {
        buff_w[i] = 0x55;
        buff_w[i+1] = 0x5A;
        buff_w[i+2] = 0xA5;
        buff_w[i+3] = 0xAA;
    }
    spiflash_write(0, 0, buff_w, 256, &retlen);
    spiflash_read(0, 0, buff_r, 256, &retlen);
    for(i = 0;i < 256; i++) {
        printf("The %d is %d\n",i , buff_r[i]);
    }
#endif                                                                                
#if 1
//        memset(buff_r, 0xfb, WRITE_NUM);
        gpio_set_reuse(GPIOD, 0xff, CK_GPIO_BEHARDWARE);
//        for(i = 0;i < 256;i += 4) {
//            buff_w[i] = 0x55;
//            buff_w[i+1] = 0x5A;
//            buff_w[i+2] = 0xA5;
//            buff_w[i+3] = 0xAA;
//        }
        spiflash_read_id(buff_r);
//        spiflash_erase(0, ERASE_BY_SECTOR,ERASE_ADDR, ERASE_LENGTH);
//        printf("the erase is ok\r\n");
        for (i = 0; i < (0x3400 + 255)/256; i++) {
            spiflash_read(0, READ_ADDR + (i * WRITE_NUM), ddr_base + (i * WRITE_NUM) , WRITE_NUM, &retlen);
        }

        uint32_t sram_bseaddr = 0x0;
        void (*fp)(void);
        uart_close((uint32_t)0x1fba4000);
        fp = (void (*)(void ))(*((uint32_t *)(sram_bseaddr)));
        (*fp)();
//        for(i=0; i<WRITE_NUM; i++)
//        {
//            printf("the data[%d] after erase is %x\r\n",i, buff_r[i]);
//        }
#if 0
//        memset(buff_w, 0x77, WRITE_NUM);
//        memset(buff_r, 0xfb, WRITE_NUM);
        spiflash_write(0, WRITE_ADDR, buff_w, WRITE_NUM, &retlen);
        spiflash_read(0, WRITE_ADDR, buff_r , WRITE_NUM, &retlen);
        for(i=0; i<WRITE_NUM; i++) {
            if(buff_w[i] != buff_r[i]) {
                break;
            }
        }
        if(i != WRITE_NUM) {
            printf("the write is failed\r\n");
        }else {
            printf("the write is OK\r\n");
        }
        for(i=0; i<WRITE_NUM; i++) {
            printf("the data[%d] after program is %x\r\n",i, buff_r[i]);
        }
#endif
#endif
//        break;
//        case 0x2:
#if 0
        /* The mode of emmc */
        for (i = 0; i < 512; i++) {
            buff_w[i] = i;
        }
        emmc_emmc_write(0, 0, 512, buff_w);
        emmc_emmc_read(0, 0, 512, buff_r);
        for(i = 0;i < 512;i++) {
            printf("The emmc %d is %d\n",i , buff_r[i]);
        }
#endif
//        break;
//        case 0x3:
        /* Nothing to do */
//        break;
//        default:
//        break;
//    }
#else
    printf("Wellcome to u-boot\n");
#endif
    while(1);
}

