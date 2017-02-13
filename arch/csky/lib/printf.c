/******************************************************************************
 * @file     printf.c
 * @brief    The File for the printf function
 * @version  V1.0
 * @date     23. Dec 2016
 ******************************************************************************/
/* ---------------------------------------------------------------------------
 * Copyright (C) 2016 CSKY Limited. All rights reserved.
 *
 * Redistribution and use of this software in source and binary forms,
 * with or without modification, are permitted provided that the following
 * conditions are met:
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   * Neither the name of CSKY Ltd. nor the names of CSKY's contributors may
 *     be used to endorse or promote products derived from this software without
 *     specific prior written permission of CSKY Ltd.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 * -------------------------------------------------------------------------- */
#include <linux/string.h>
#include <configs/eragon.h>
#include <asm/arch-eragon/uart.h>
#include <common.h>
//#include <linux/stdarg.h>

/******************************************************
*change the number to string
*
* uq : the number need to change
* base: the base of system(for example 2,8,10,16 as usual)
* buf: data buffer for change procedure and return result
* return value: the string  after change
*********************************************************/
static char *numtostring (unsigned int uq, int base, char *buf )
{
  register char *p, *p0;
  int n = 0, i;

  p = buf;
  *buf = 0;
  do
  {
    *buf ++ = "0123456789abcdef"[uq % base];
    n++;
  } while (uq /= base);
  p[n] = '\0';

  p0 = ++buf;
  if (base == 16 && n < 8)  //If Hex, the length is fixxed with 8 digitals
  {
    for (i = 0; i < 8 - n; i++)
    {
      p0[i] = '0';
    }
    for (; i < 8; i++)
    {
      p0[i] = p[8 - i - 1];
    }
    p0[8] = '\0';
  }
  else
  {
    for (i = 0; i < n; i++)
    {
      p0[i] = p[n - i - 1];
    }
    p0[n] = '\0';
  }

  return (p0);
}

#ifdef CROSS_COMPILE

/*
 * display the char on the console
 * ch: the char need to display
 */
int putchar(int ch)
{
  while(uart_put_char(CONSOLE_UART_BASE, ch) != SUCCESS);
  return 0;
}

static int puts_without_n(const char *ptr)
{
   while(*ptr !='\0')
   {
     if (SUCCESS == uart_put_char(CONSOLE_UART_BASE, *ptr))
       ptr++;
   }
   return 0;
}

/*
 * display a string on the console
 * ptr: the string need to display
 */
void puts(const char *s)
{
   puts_without_n(s);
   uart_put_char(CONSOLE_UART_BASE, '\n');
   return 0;
}

/*
 * receive a char from the console
 *return value: the char received from the console
 */
int getchar(void)
{
  char ch;
  while(uart_get_char(CONSOLE_UART_BASE, (uint8_t *)&ch) != SUCCESS);
  return ch;
}

/*
 * print the result after translated according to the format
 */
int printf ( const char *fmt, ... )
{
  const char *s;
  int        value;
  uint32_t        ptr;
  char       ch, buf[64], *pbuf;
  va_list    ap;

  va_start(ap, fmt);
  while (*fmt)
  {
    if (*fmt != '%')
    {
      putchar(*fmt++);
      continue;
    }

    switch (*++fmt)
    {
      case 's':
        s = va_arg(ap, const char *);
         puts_without_n(s);
         break;
      case 'd':
         value = va_arg(ap, int);
         if (value < 0)
         {
            putchar('-');
            value = 0 - value;
         }
         pbuf = numtostring((unsigned int)value, 10, buf);
         puts_without_n(pbuf);
         break;
       case 'x':
         value = va_arg(ap,int);
         pbuf = numtostring((unsigned int)value, 16, buf);
         puts_without_n(pbuf);
         break;
       case 'c':
          ch = (unsigned char)va_arg(ap, int);
          pbuf = &ch;
          putchar(*pbuf);
          break;
       case 'p':
         ptr = (unsigned) va_arg(ap, void *);
         pbuf = numtostring(ptr, 16, buf);
         puts_without_n(pbuf);
         break;
       default:
         putchar(*fmt);
         break;
    }
    fmt ++;
  }
  va_end(ap);
  return 0x01;
}
#endif

