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

extern s32 uart_open( u32 uart_addrbase);
extern void sdram_init(void);
#define READ_ADDR 0x3000
#define READ_ADDR_LOONGSON_SPL 0x40000 /* address at 256K */
#define READ_ADDR_LOONGSON_UBOOT 0x43000 /* 256K + 12K */
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
	u32	dram_baseaddr = 0x179d7000;
	void      (*fp)(void);
	u32  ret;

	mini_printf("The U-Boot-spl start.\n");
	mini_printf("U-Boot version is 2016.07, internal version is 0.4\n");
	gpio_set_reuse(GPIOD, 0xff, GPIO_BEHARDWARE);
	om_judge = get_boot_select();

	switch(om_judge) {
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

		for (i = 0; i < (CONFIG_SPL_SIZE + 511) / 512; i++) {
			emmc_emmc_read(0, (READ_ADDR_LOONGSON_SPL + (i * 512))/0x200,\
			512, (u8 *)(CONFIG_LOONGSON_SRAM_BASE + (i * 512)));
		}

		for (i = 0; i < (CONFIG_UBOOT_SIZE + 511) / 512; i++) {
			emmc_emmc_read(0, (READ_ADDR_LOONGSON_UBOOT + (i * 512))/0x200,\
			512, (u8 *)(CONFIG_LOONGSON_DDR_BASE + (i * 512)));
		}

		i = readl(CHIP_CTRLPMU + 0x38);
		i |= (1 << 9);
		writel(i, CHIP_CTRLPMU + 0x38);
		mini_printf("Loongson wake up.\n");

		fp = (void (*)(void ))(*((u32 *)(dram_baseaddr)));
		(*fp)();
		break;
		default:
		mini_printf("OM mode is %x, please check the OM.\n", om_judge);
		break;
	}
	while(1);
}

