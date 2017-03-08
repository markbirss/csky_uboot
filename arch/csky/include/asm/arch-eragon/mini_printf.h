#ifndef __MINI_PRINTF
#define __MINI_PRINTF
#include <common.h>
/*
 * SPDX-License-Identifier:     GPL-2.0+
 */
void    mini_putc(const char c);
void    mini_puts(const char *s);
int     mini_printf(const char *fmt, ...)
                __attribute__ ((format (__printf__, 1, 2)));
int     mini_vprintf(const char *fmt, va_list args);
#endif
