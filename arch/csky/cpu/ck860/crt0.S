/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <config.h>
#include <asm-offsets.h>
#include <linux/linkage.h>
#include <asm/arch/mmu.h>
.import start_cskyboot
.import __text_start
.global _main
_main:

/*
 * Set up initial C runtime environment and call board_init_f(0).
 */

#ifdef CONFIG_SPL_BUILD
    lrw     r3, CONFIG_SPL_STACK
#else
    lrw     r3, CONFIG_SYS_INIT_SP_ADDR
#endif
    mov     sp, r3

    mov     r0, sp
    jbsr    board_init_f_alloc_reserve

    mov     sp, r0
    /* set up gd here, outside any C code */
    mov     r28, r0
    jbsr    board_init_f_init_reserve


#ifndef CONFIG_SPL_BUILD
    /* Setup the initial page tables */
    create_page_table  __text_start

    /* enable MMU */
    csky_mmu_enable

    /* open prefetch */
//    mfcr    a3, cr31
//    bseti   a3, 4
//    mtcr    a3, cr31
#endif


__clear_bss:
    lrw     r3, __bss_start /* Get start of bss from linking script file */
    lrw     r2, __bss_end   /* Get end of bss from linking script file */
    subu    r2, r3          /* Calculate size of bss */
    lsri    r2, 2           /* Size of whole words */
    cmpnei  r2, 0
    bf      __goto_init_f
    movi    r1, 0           /* Set zero value to write */

1:
    stw     r1, (r3)        /* Zero next word */
    addi    r3, 4           /* Increase bss pointer */
    decne   r2              /* Decrease counter */
    bt      1b              /* Repeat for all bss */

__goto_init_f:
#ifndef CONFIG_SPL_BUILD
    movi    r0, 0x00800     /* To skip the relocate code */
    jbsr    board_init_f

    lrw     r3, GD_START_ADDR_SP
    add     r3, r28
    ldw     sp, (r3)

    mov     r0, r28
    lrw     r3, GD_RELOCADDR
    add     r3, r28
    ldw     r1, (r3)

    jbsr    board_init_r
    rts
#else
    jbsr    board_init_f
    jbsr    board_init_r
#endif

