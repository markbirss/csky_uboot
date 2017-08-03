/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/arch/hardware.h>
#include <asm/arch/om.h>
#include <asm/datatype.h>

/*
 * get the om pin value
 *
 */
s8 get_boot_select(void)
{
   int8_t *bootsel = (s8 *)OM_BASEADDR;

   return (*bootsel);
}

