/*
 * SPDX-License-Identifier: GPL-2.0+
 */
#include <config.h>
#include <asm-offsets.h>

.export	csky_vsr_table	/* Vector table base address. */
.globl  _start
.import start_cskyboot
.import __stack
.import __fstack
/* Vector table space. */
.align 10
csky_vsr_table:
        .long _start
.align 10
.rept 63
        .long hw_vsr_default_exception
.endr


.text
_start:

	/* Init psr value */
	mfcr	a3, psr
    bseti   a3, 8
    bseti   a3, 5
    bseti   a3, 31
    mtcr    a3, psr

	/*
	 * Setup initial vector base table for interrupts and exceptions
	 */
	lrw	    a2, csky_vsr_table
	mtcr	a2, vbr

    /* Initialize the normal stack pointer from the linker definition. */
	lrw     a3, __stack
	mov	    sp, a3

    /* Initialize the fast interrupt stack pointer . */
    psrset  af
    lrw     a3, __fstack
    mov     sp, a3
    psrclr  af
    /* Initial CKCPU MMU & Cache */
    movi    a3, 0x0
    mtcr    a3, cr18

    /*
     * Step 3: enable instruction & data cache
     */
    /* Invalid Instruction and data cache content */
    mfcr    r4, cr17
    bseti   r4, 1
    bseti   r4, 2
    bseti   r4, 4
    mtcr    r4, cr17

    /* Enable instruction and data cache */
    mfcr    r3, cr18
    bseti   r3, 2
    bseti   r3, 3
    mtcr    r3, cr18

__clear_bss:
    lrw     a3, __bss_start /* Get start of bss from linking script file */
    lrw     a2, __bss_end   /* Get end of bss from linking script file */
    subu    a2, a3          /* Calculate size of bss */
    lsri    a2, 2           /* Size of whole words */
    cmpnei  r2, 0
    bf      __goto_c
    movi    a1, 0           /* Set zero value to write */

1:
    stw     a1, (a3)        /* Zero next word */
    addi    a3, 4           /* Increase bss pointer */
    decne   a2              /* Decrease counter */
    bt      1b              /* Repeat for all bss */

__goto_c:
	jbsr	start_cskyboot

__exit:
    bkpt
	br	    __exit

