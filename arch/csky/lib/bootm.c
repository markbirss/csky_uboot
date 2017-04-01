/*
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
#include <asm/bootm.h>
#include <linux/compiler.h>
#include <bootm.h>
#include <vxworks.h>
#include <asm/arch-eragon/interrupt.h>
//extern csky_cache_enbale();
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
	void   (*theKernel)(int magic, void * params);
	theKernel = (void (*)(int, void *))images->ep;
	printf("\nStarting kernel ... \n\n");

	disable_interrupts();
//	csky_cache_enable();
	flush_cache(0,0);
	theKernel (0x20150401, 0x8f000000);
	return 1;
}
