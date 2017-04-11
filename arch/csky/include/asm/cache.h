/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ASM_CACHE_H
#define __ASM_CACHE_H

#include <asm/system.h>

/*
 * The current upper bound for ARM L1 data cache line sizes is 64 bytes.  We
 * use that value for aligning DMA buffers unless the board config has specified
 * an alternate cache line size.
 */
#ifdef CONFIG_SYS_CACHELINE_SIZE
#define ARCH_DMA_MINALIGN	CONFIG_SYS_CACHELINE_SIZE
#else
#define ARCH_DMA_MINALIGN	64
#endif
#endif /* __ASM_CACHE_H */
