/*
 * (C) Copyright 2002-2010
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef	__ASM_GBL_DATA_H
#define __ASM_GBL_DATA_H

/* Architecture-specific global data */
struct arch_global_data {
	unsigned long timer_rate_hz;
	unsigned long tbu;
	unsigned long tbl;
	unsigned long lastinc;
};

#include <asm-generic/global_data.h>

#if 0
#define DECLARE_GLOBAL_DATA_PTR
#define gd	get_gd()

static inline gd_t *get_gd(void)
{
	gd_t *gd_ptr;

	__asm__ volatile("mfcr %0, cr<9, 0>\n" : "=r" (gd_ptr));

	return gd_ptr;
}
#endif

#define DECLARE_GLOBAL_DATA_PTR register volatile gd_t *gd asm ("r28")

#endif /* __ASM_GBL_DATA_H */
