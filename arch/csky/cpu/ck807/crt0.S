/*
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <config.h>
#include <asm-offsets.h>
#include <linux/linkage.h>

/*
 * This file handles the target-independent stages of the U-Boot
 * start-up where a C runtime environment is needed. Its entry point
 * is _main and is branched into from the target's start.S file.
 *
 * _main execution sequence is:
 *
 * 1. Set up initial environment for calling board_init_f().
 *    This environment only provides a stack and a place to store
 *    the GD ('global data') structure, both located in some readily
 *    available RAM (SRAM, locked cache...). In this context, VARIABLE
 *    global data, initialized or not (BSS), are UNAVAILABLE; only
 *    CONSTANT initialized data are available. GD should be zeroed
 *    before board_init_f() is called.
 *
 * 2. Call board_init_f(). This function prepares the hardware for
 *    execution from system RAM (DRAM, DDR...) As system RAM may not
 *    be available yet, , board_init_f() must use the current GD to
 *    store any data which must be passed on to later stages. These
 *    data include the relocation destination, the future stack, and
 *    the future GD location.
 *
 * 3. Set up intermediate environment where the stack and GD are the
 *    ones allocated by board_init_f() in system RAM, but BSS and
 *    initialized non-const data are still not available.
 *
 * 4a.For U-Boot proper (not SPL), call relocate_code(). This function
 *    relocates U-Boot from its current location into the relocation
 *    destination computed by board_init_f().
 *
 * 4b.For SPL, board_init_f() just returns (to crt0). There is no
 *    code relocation in SPL.
 *
 * 5. Set up final environment for calling board_init_r(). This
 *    environment has BSS (initialized to 0), initialized non-const
 *    data (initialized to their intended value), and stack in system
 *    RAM (for SPL moving the stack and GD into RAM is optional - see
 *    CONFIG_SPL_STACK_R). GD has retained values set by board_init_f().
 *
 * 6. For U-Boot proper (not SPL), some CPUs have some work left to do
 *    at this point regarding memory, so call c_runtime_cpu_setup.
 *
 * 7. Branch to board_init_r().
 *
 * For more information see 'Board Initialisation Flow in README.
 */
#include <asm/arch-eragon/mmu.h>
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
    mfcr    a3, cr31
    bseti   a3, 4
    mtcr    a3, cr31
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

