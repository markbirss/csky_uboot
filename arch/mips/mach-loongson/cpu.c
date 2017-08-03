/*
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <dm.h>
#include <asm/types.h>
#include <config.h>
#include <asm-generic/global_data.h>

DECLARE_GLOBAL_DATA_PTR;

phys_size_t initdram(int board_type)
{
	gd->flags = GD_FLG_SKIP_RELOC;
	return PHYS_SDRAM_1_SIZE;
}
