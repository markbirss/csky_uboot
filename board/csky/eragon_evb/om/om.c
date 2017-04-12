/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <configs/eragon.h>
#include <asm/arch-eragon/om.h>
#include <asm/arch-eragon/datatype.h>

/*
 * get the om pin value
 *
 */
int8_t get_boot_select(void)
{
   int8_t *bootsel = (int8_t *)CK_OM_ADDRBASE;

   return (*bootsel);
}

