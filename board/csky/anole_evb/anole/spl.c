/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <linux/types.h>
#include <common.h>
#include <console.h>
#include <asm/io.h>
#include <spl.h>
#include <asm/arch/hardware.h>
#include <asm/spl.h>
#include <asm/datatype.h>
#include <asm/mini_printf.h>
#include <asm/arch/gpio.h>
#include <asm/mmc/emmc.h>
#include <asm/arch/om.h>
#include <asm/spiflash.h>

extern s32 uart_open( u32 uart_addrbase);
extern void sdram_init(void);
#define READ_ADDR 0x3000

void board_init_f(ulong dummy)
{
	/* Clear global data */
	uart_open(CONSOLE_UART_BASE);
	sdram_init();
	mini_printf("Wellcome to SPL!\n");
}

void board_init_r(gd_t *gd, ulong dest_addr)
{
	int	i;
	s8	om_judge;
	/* Because of the relocation of uboot, the address of uboot in DDR will change.
	   So we prepare the uboot at the address which is calculated by uboot itself.
	   Different DDR address and size will create different uboot address. */
	u8	*ddr_base = (u8 *)CONFIG_SYS_TEXT_BASE;
	u32	dram_baseaddr = CONFIG_SYS_TEXT_BASE;
	void	(*fp)(void);
	u32	retlen;
	u32	ret;

	mini_printf("The U-Boot-spl start.\n");
	mini_printf("U-Boot version is 2016.07, internal version is %s\n", UBOOT_INTERNAL_VERSION);
	gpio_set_reuse(GPIOD, 0xff, GPIO_BEHARDWARE);
	om_judge = get_boot_select();

	switch(om_judge) {
	case 0x1:
	/* The mode of spi flash */
		mini_printf("This is spiflash mode.\n");
		for (i = 0; i < (CONFIG_UBOOT_SIZE + 255)/256; i++) {
			spiflash_read(0, READ_ADDR + (i * 256), ddr_base + (i * 256) , 256, &retlen);
		}
		fp = (void (*)(void ))(*((uint32_t *)(dram_baseaddr)));
		(*fp)();
		break;
	case 0x2:
	/* The mode of emmc */
		mini_printf("This is eMMC mode.\n");
		ret = emmc_host_init(NULL);
		if (ret != 0) {
			mini_printf("The eMMC is not exist.\n");
			break;
		}
		mini_printf("eMMC init ready.\n");
		for (i = 0; i < (CONFIG_UBOOT_SIZE + 511) / 512; i++) {
			emmc_emmc_read(0, (READ_ADDR + (i * 512))/0x200, 512, (u8 *)(dram_baseaddr + (i * 512)));
		}
		fp = (void (*)(void ))(*((u32 *)(dram_baseaddr)));
		(*fp)();
		break;
	default:
		mini_printf("OM mode is %x, please check the OM.\n", om_judge);
		break;
	}
	while(1);
}

