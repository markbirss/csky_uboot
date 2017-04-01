/*
 * Copyright (c) 2017 CSKY, Inc
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <netdev.h>

DECLARE_GLOBAL_DATA_PTR;

int do_reset(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	return 1;
}

void relocate_code(ulong start_addr_sp, gd_t *new_gd, ulong relocaaddr)
{
	/* TODO: Implement this */
	while (1);
}

