/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/mmc/emmc_interface.h>
#include <asm/arch/hardware.h>

#define CEATA_MAP	0xffffffff
static u32 base_adr = EMMC_BASEADDR;

/*
 * Set the particular bits of the specified register.
 * @param[in] reg	The particular register to which the bits are to be set
 * @param[in] val	The bitmask for the bits which are to be set.
 * return 	The new value of the register
 */
u32 emmc_set_bits(controller_reg_e reg, u32 val)
{
    u32 *reg_addr;
    reg_addr = (u32 *)(base_adr + reg);

    *reg_addr |= val;
    return *reg_addr;
}

/*
 * Clear the particular bits of the specified register.
 * @param[in] reg	The particular register to which the bits are to be cleared.
 * @param[in] val	The bitmask for the bits which are to be cleared.
 * return 	The new value of the register
 */
u32 emmc_clear_bits(controller_reg_e reg, u32 val)
{
    u32 *reg_addr;
    reg_addr = (u32 *)(base_adr + reg);

    *reg_addr &= (~val);
    return *reg_addr;
}

void plat_loop(u32 value)
{
    u32 counter;
    int i;
    volatile int j;

    for (counter = 0; counter < (value * 5); counter++)
        for (i = 0; i < 100; i++) {
            j = i;
        }
}
/*
 * Set the value of the specified register.
 * @param[in] reg	The particular register for which values are to be set.
 * @param[in] val	The value.
 * return 	The new value of the register
 */
u32 emmc_set_register(controller_reg_e reg, u32 val)
{
    u32 *reg_addr;
    reg_addr = (u32 *)(base_adr + reg);

    *reg_addr = val;
    return *reg_addr;
}

/*
 * Read the value of the specified register.
 * @param[in] reg	The particular register which is to be read.
 * return 	The value of the register
 */
u32 emmc_read_register(controller_reg_e reg)
{
    u32 *reg_addr;
    u32 retval;

    reg_addr = (u32 *)(base_adr + reg);
    retval = *reg_addr;

    /* if request is to read CDETECT, make sure you return
       the hacked ;) CEATA_MAP bit */
    if (CDETECT == reg) {
        retval &= CEATA_MAP;
    }

    return retval;
}

