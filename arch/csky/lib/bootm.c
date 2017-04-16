/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <image.h>
#include <u-boot/zlib.h>
#include <asm/byteorder.h>
#include <libfdt.h>
#include <mapmem.h>
#include <fdt_support.h>
#include <linux/compiler.h>
#include <bootm.h>
#include <vxworks.h>
#include <asm/arch/interrupt.h>
DECLARE_GLOBAL_DATA_PTR;

/* Main Entry point for arm bootm implementation
 *
 * Modeled after the powerpc implementation
 * DIFFERENCE: Instead of calling prep and go at the end
 * they are called if subcommand is equal 0.
 */
int do_bootm_linux(int flag, int argc, char * const argv[],
		   bootm_headers_t *images)
{
	void (*theKernel)(int magic, void * params);
	char *tmp;
	unsigned int dtb_load_addr;

	if ((tmp = getenv("dtb_load_addr_virt")) != NULL) {
		dtb_load_addr = simple_strtoul(tmp, NULL, 16);
		printf("dtb_load_addr: 0x%X\n", dtb_load_addr);
	} else {
		printf("\nError! No dtb address found! Stop here...\n");
		while (1);
	}

	theKernel = (void (*)(int, void *))images->ep;
	printf("\nStarting kernel ... \n\n");

	disable_interrupts();
	flush_cache(0,0);
	theKernel (0x20150401, (void *)dtb_load_addr);
	return 1;
}
