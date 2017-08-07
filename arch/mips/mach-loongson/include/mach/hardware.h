/*
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __MACH_HARDWARE_H__
#define __MACH_HARDWARE_H__
#include <asm/types.h>

#ifdef CONFIG_SPL_BUILD
#define TIMER_BASEADDR       (u32)(0x1fbb0000)
#define MAILBOX_BASEADDR     (u32)(0x1fbd4000)
#else
#define TIMER_BASEADDR       (u32)(0xbfbb0000)
#define MAILBOX_BASEADDR     (u32)(0xbfbd4000)
#endif /* CONFIG_SPL_BUILD */

#define SYS_FREQ             CONFIG_SYS_CLK_FREQ

#endif /* __MACH_HARDWARE_H__ */
