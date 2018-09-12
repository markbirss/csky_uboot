/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/clock.h>
#include <asm/datatype.h>
#include <asm/arch/hardware.h>

int arch_cpu_init(void)
{
    int freq;
#ifdef CONFIG_BOOT_SLAVE_CPU
    if (clock_set_cpu_speed(CLOCK_CPU_FREQ_700M) != 0) {
#else
    if (clock_set_cpu_speed(CLOCK_CPU_FREQ_800M) != 0) {
#endif
        return -1;
    }
    /* Change the LSP to 118M */
    volatile u32 *chip_ctrl = (volatile u32 *)(CHIP_CTRL_BASEADDR + 0x4);
    *chip_ctrl = 0x0;
    /* To do the next */
    return 0;
}
