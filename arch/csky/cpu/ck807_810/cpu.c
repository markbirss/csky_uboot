/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <netdev.h>
#include <asm/arch/hardware.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

int do_reset(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	u32 temp;
	temp = readl(CHIP_CTRL_BASEADDR+0xa0000000+SOFTWARE_RESET_CONTROL);
	temp = temp & 0x1;
	writel(temp, CHIP_CTRL_BASEADDR+0xa0000000+SOFTWARE_RESET_CONTROL);
	return 0;
}

void relocate_code(ulong start_addr_sp, gd_t *new_gd, ulong relocaaddr)
{
	/* TODO: Implement this */
	while (1);
}

