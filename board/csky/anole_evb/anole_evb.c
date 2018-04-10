/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/arch/hardware.h>
#include <asm/arch/gpio.h>
#include <asm/mini_printf.h>
#include <asm/arch/ddr.h>
#include <common.h>
#include <mmc.h>
#include <miiphy.h>
#include <netdev.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_DWMMC
extern int eragon_dwmci_add_port(int index, u32 regbase, int bus_width);
#endif

#ifdef CONFIG_SPL_BUILD
void sdram_init(void)
{
	init_ddr();
}
#endif
#ifdef CONFIG_GENERIC_MMC
static int init_dwmmc(void)
{
	int ret = 1;
#ifdef CONFIG_DWMMC
	ret &= eragon_dwmci_add_port(0, ERAGON_MMC0_BASE, 8);
	if (ret)
#ifdef CONFIG_SPL_BUILD
		mini_printf("Error adding eMMC port (%d)\n", ret);
#else
		printf("Error adding eMMC port (%d)\n", ret);
#endif
#endif
	return ret;
}

int board_mmc_init(bd_t *bis)
{
	int ret;
	/* add the eMMC and sd ports */
	ret = init_dwmmc();
	if (ret)
#ifdef CONFIG_SPL_BUILD
		mini_printf("init_dwmmc failed\n");
#else
		printf("init_dwmmc failed\n");
#endif
	return ret;
}
#endif
int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;
	return 0;
}
void show_boot_progress(int val) {}

int board_early_init_f(void)
{
	/* Use the UART 2 */
	gpio_set_reuse(GPIOB, 0x3, GPIO_BEHARDWARE);
	gpio_set_reuse(GPIOB, 0x30, GPIO_BEHARDWARE);
	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1 + 0x14000000;
	gd->bd->bi_dram[0].size = 0x4000000;
}

#ifndef CONFIG_SPL_BUILD
#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int ret = 0;

#if defined(CONFIG_ETH_DESIGNWARE)
	ret = designware_initialize(GMAC_BASEADDR, PHY_INTERFACE_MODE_MII);
#endif
	return ret;
}
#endif
#endif

int board_early_init_r(void)
{
	timer_init();
	return 0;
}

