/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <asm-offsets.h>
#include <linux/linkage.h>
#include <asm/arch/mmu.h>
#include <asm/arch/hardware.h>
.export hw_vsr_autovec
.export csky_vsr_table      /* Vector table base address. */
.globl  _start
.import start_cskyboot
.import hw_vsr_default_exception
.import __stack
.import __fstack
.data
/* Vector table space. */
.section .exptable
.align 10
csky_vsr_table:
.long _start
.rept 31
.long hw_vsr_default_exception
.endr
.rept 64
.long hw_vsr_autovec
.endr


.text
_start:

    /* Init psr value */
    mfcr    r3, psr
    bseti   r3, 8
    bseti   r3, 5
    bseti   r3, 31
    mtcr    r3, psr

    /*
     * Setup initial vector base table for interrupts and exceptions
     */
    lrw     r2, csky_vsr_table
    mtcr    r2, vbr

    /* Initial CKCPU MMU & Cache */
    csky_mmu_disable

    /* Invalid Instruction and data cache content */
    mfcr    r4, cr17
    bseti   r4, 0
    bseti   r4, 1
    bseti   r4, 4
    bseti   r4, 5
    mtcr    r4, cr17

    mfcr    r4, cr31
#ifdef CONFIG_CK807
    bseti   r4, 4
#endif
#ifdef CONFIG_CK810
    bseti   r4, 2
    bseti   r4, 8
#endif
    mtcr    r4, cr31

#ifdef CONFIG_FRQ_UP
#ifdef CONFIG_IS_ASIC
    /* cpucclk = pmuclk = 24MHz */
    lrw     r2, MPU_BASEADDR
    movi    r3, 0x306
    st.w    r3, (r2, 0x00)

    /* Power down PLL */
    lrw     r2, MPU_BASEADDR
    movi    r3, 0x43
    st.w    r3, (r2, 0x04)

    lrw     r2, MPU_BASEADDR
    movi    r3, 0x001e
    st.w    r3, (r2, 0x00)

    /* Update PLL */
    lrw     r2, MPU_BASEADDR
    lrw     r3, 0x00000000
    st.w    r3, (r2, 0x08)

    lrw     r2, MPU_BASEADDR
    movi    r3, 0x316
    st.w    r3, (r2, 0x00)

    /* Power on PLL */
    lrw     r2, MPU_BASEADDR
    movi    r3, 0x40
    st.w    r3, (r2, 0x04)

    lrw     r2, MPU_BASEADDR
    movi    r3, 0x31e
    st.w    r3, (r2, 0x00)

WAIT:
    lrw     r2, MPU_BASEADDR
    ld.w    r3, (r2, 0x38)
    btsti   r3, 8
    bf      WAIT

    /* cpucclk = cpupll = 800MHz */
    lrw     r2, MPU_BASEADDR
    movi    r3, 0x006
    st.w    r3, (r2, 0x00)


    lrw     r2, CHIP_CTRL_BASEADDR
    lrw     r3, 0x11000
    st.w    r3, (r2, 0x00)

    lrw     r2, CHIP_CTRL_BASEADDR
    lrw     r3, 0x0
    st.w    r3, (r2, 0x04)
#endif /* CONFIG_IS_ASIC */
#endif /* CONFIG_FRQ_UP */

    jbsr    _main

__exit:
    bkpt
    br      __exit

