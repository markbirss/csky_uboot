/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <command.h>
#include <asm/clock.h>
#include <asm/datatype.h>

static int do_set_frequency(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int freq = 0;

	if (2 == argc) {
		freq = simple_strtoul(argv[1], NULL, 10);
	} else {
		printf("Please input the frequency of cpu.\n");
		return -1;
	}
	if (clock_set_cpu_speed(freq) != 0) {
		printf("CSKY cpu frequency set failed\n");
		return -1;
	} else {
		printf("CSKY cpu frequency set success\n");
	}
	return 0;
}

static int do_get_frequency(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	uint32_t freq;
	if (clock_get_cpu_speed(&freq) != 0) {
		printf("CSKY cpu frequency get failed\n");
		return -1;
	} else {
		printf("CSKY cpu frequency is %d\n", freq);
	}
	return 0;
}

U_BOOT_CMD(
	set_cpu_freq,   2,      1,      do_set_frequency,
	"[frequency] - the frequency of cpu",
	"\n    - Set the cpu frequency\n"
);

U_BOOT_CMD(
	get_cpu_freq,   1,      1,      do_get_frequency,
	"[frequency] - the frequency of cpu",
	"\n    - Get the cpu frequency.\n"
);
