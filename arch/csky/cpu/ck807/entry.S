/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include "config.h"

.global hw_vsr_default_exception
.global hw_vsr_autovec
.global hw_vsr_fastautovec
.global hw_vsr_reset
.global hw_vsr_tlbmiss
.extern csky_default_exception_handler
.extern ck_irq_service

        .text
        .balign 4
hw_vsr_default_exception:
        subi    sp, 124             /* Preserve the entire state. */
        stw     r0, (sp, 0)
        stw     r1, (sp, 4)
        stw     r2, (sp, 8)
        stw     r3, (sp, 12)
        stw     r4, (sp, 16)
        stw     r5, (sp, 20)
        stw     r6, (sp, 24)
        stw     r7, (sp, 28)
        stw     r8, (sp, 32)
        stw     r9, (sp, 36)
        stw     r10, (sp, 40)
        stw     r11, (sp, 44)
        stw     r12, (sp, 48)
        stw     r13, (sp, 52)
        stw     r15, (sp, 56)
        stw     r16, (sp, 60)
        stw     r17, (sp, 64)
        stw     r18, (sp, 68)
        stw     r19, (sp, 72)
        stw     r20, (sp, 76)
        stw     r21, (sp, 80)
        stw     r22, (sp, 84)
        stw     r23, (sp, 88)
        stw     r24, (sp, 92)
        stw     r25, (sp, 96)
        stw     r26, (sp, 100)
        stw     r27, (sp, 104)
        stw     r28, (sp, 108)
        stw     r29, (sp, 112)
        stw     r30, (sp, 116)
        stw     r31, (sp, 120)

        subi    sp, 8
        mfcr    r2, epsr
        stw     r2, (sp,4)         /* save epsr registwr */
        mfcr    r2, epc
        stw     r2, (sp,0)         /* save epc register */

        mfcr    r0, psr            /* Get psr register */
        lsri    r0, 16             /* Get vector in 7 bits */
        sextb   r0                 /* Fill upper bytes with zero */
        mov     r1, sp             /* pass stack point to r3*/

        jsri    csky_default_exception_handler
        /* exception   VSR.*/

        ldw     r2, (sp,0)
        mtcr    r2, epc            /* restore the epc */
        ldw     r2, (sp,4)
        mtcr    r2, epsr           /* restore the epsr */
        addi    sp, 8
        ldm     r0-r13,(sp)        /* restore all the regs */
        addi    sp,56
        ldm     r15-r31,(sp)
        addi    sp,68

        rte

/*********************************************************************/
/*      Normal interrupt vector handler.                             */
/*********************************************************************/
        .text
        .balign 4
hw_vsr_autovec:
        subi    sp, 124             /* Preserve the entire state. */
        stw     r0, (sp, 0)
        stw     r1, (sp, 4)
        stw     r2, (sp, 8)
        stw     r3, (sp, 12)
        stw     r4, (sp, 16)
        stw     r5, (sp, 20)
        stw     r6, (sp, 24)
        stw     r7, (sp, 28)
        stw     r8, (sp, 32)
        stw     r9, (sp, 36)
        stw     r10, (sp, 40)
        stw     r11, (sp, 44)
        stw     r12, (sp, 48)
        stw     r13, (sp, 52)
        stw     r15, (sp, 56)
        stw     r16, (sp, 60)
        stw     r17, (sp, 64)
        stw     r18, (sp, 68)
        stw     r19, (sp, 72)
        stw     r20, (sp, 76)
        stw     r21, (sp, 80)
        stw     r22, (sp, 84)
        stw     r23, (sp, 88)
        stw     r24, (sp, 92)
        stw     r25, (sp, 96)
        stw     r26, (sp, 100)
        stw     r27, (sp, 104)
        stw     r28, (sp, 108)
        stw     r29, (sp, 112)
        stw     r30, (sp, 116)
        stw     r31, (sp, 120)

        subi    sp, 8
        mfcr    r2, epsr
        stw     r2, (sp,4)         /* save epsr registwr */
        mfcr    r2, epc
        stw     r2, (sp,0)         /* save epc register */

        /* read the interrupt vector number from interrupt status register */
        lrw     r4, INTC_BASEADDRESS
        ldw     r0, (r4, 0x0)
        movi    r10,0x7f
        and     r0, r10
        subi    r0, 0x20

        jsri    ck_irq_service

        ldw     r2, (sp,0)
        mtcr    r2, epc           /* restore the epc */
        ldw     r2, (sp,4)
        mtcr    r2, epsr          /* restore the epsr */
        addi    sp, 8
        ldm     r0-r13,(sp)       /* restore all the regs */
        addi    sp,56
        ldm     r15-r31,(sp)
        addi    sp,68
        rte

/****************************************************************************/
/*      Fast interrupt vector handler.                                      */
/****************************************************************************/
        .text
        .balign 4
hw_vsr_fastautovec:
        /*read the interrupt vector number from interrupt status register */
        lrw     r4, INTC_BASEADDRESS
        ldw     r0, (r4, 0x0)
        movi    r10,0x7f
        and     r0, r10

        jsri    ck_irq_service


        rfi                      /*   Restore the psr and pc     */

/****************************************************************************/
/*      reset interrupt vector handler.                                     */
/****************************************************************************/
        .text
        .balign 4
hw_vsr_reset:

        jsri    _start

        rte                      /*   Restore the psr and pc     */
