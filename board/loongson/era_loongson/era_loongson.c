/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>

DECLARE_GLOBAL_DATA_PTR;

void show_boot_progress(int val) {}

int board_early_init_f(void)
{
	return 0;
}
void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1 + 0x100000;
	gd->bd->bi_dram[0].size = 0x80000;
}

int board_early_init_r(void)
{
	return 0;
}

