/*
 * A lowlevel_init function that sets up the stack to call a C function to
 * perform further init.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <asm-offsets.h>
#include <config.h>
#include <linux/linkage.h>
.globl lowlevel_init
lowlevel_init:
    /*
     * Setup a temporary stack. Global data is not available yet.
     * Initialize the normal stack pointer from the linker definition.
     */
#ifdef CONFIG_SPL_BUILD
    lrw     r3, CONFIG_SPL_STACK
#else
    lrw     r3, CONFIG_SYS_INIT_SP_ADDR
#endif
    mov     sp, r3

    rts

