/*
 * Copyright (C) 2017 C-SKY MicroSystems Co.,Ltd.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ASM_ARCH_HARDWARE_H__
#define __ASM_ARCH_HARDWARE_H__
#include <asm/types.h>

#ifdef CONFIG_SPL_BUILD
#define INTC_BASEADDR        (u32)(0x1fba3000)
#define GPIOA_BASEADDR       (u32)(0x1fbac000)
#define GPIOB_BASEADDR       (u32)(0x1fbad000)
#define GPIOC_BASEADDR       (u32)(0x1fbae000)
#define GPIOD_BASEADDR       (u32)(0x1fbaf000)
#define TIMER_BASEADDR       (u32)(0x1fba2000)
#define GMAC_BASEADDR        (u32)(0x1fb10000)
#define EMMC_BASEADDR        (u32)(0x1fb13000)
#define OM_BASEADDR          (u32)(0x1fbd0100)
#define SPI0_BASEADDR        (u32)(0x1fb18000)
#define SPI1_BASEADDR        (u32)(0x1fbab000)
#define UART0_BASEADDR       (u32)(0x1fb14000)
#define UART1_BASEADDR       (u32)(0x1fb15000)
#define UART2_BASEADDR       (u32)(0x1fba4000)
#define UART3_BASEADDR       (u32)(0x1fba5000)
#define UART4_BASEADDR       (u32)(0x1fba6000)
#define MAILBOX_BASEADDR     (u32)(0x1fbd4000)
#define CHIP_CTRLPMU         (u32)(0x1fbd0000)

#else

#define INTC_BASEADDR        (u32)(0xbfba3000)
#define GPIOA_BASEADDR       (u32)(0xbfbac000)
#define GPIOB_BASEADDR       (u32)(0xbfbad000)
#define GPIOC_BASEADDR       (u32)(0xbfbae000)
#define GPIOD_BASEADDR       (u32)(0xbfbaf000)
#define TIMER_BASEADDR       (u32)(0xbfba2000)
#define GMAC_BASEADDR        (u32)(0xbfb10000)
#define EMMC_BASEADDR        (u32)(0xbfb13000)
#define OM_BASEADDR          (u32)(0xbfbd0100)
#define SPI0_BASEADDR        (u32)(0xbfb18000)
#define SPI1_BASEADDR        (u32)(0xbfbab000)
#define UART0_BASEADDR       (u32)(0xbfb14000)
#define UART1_BASEADDR       (u32)(0xbfb15000)
#define UART2_BASEADDR       (u32)(0xbfba4000)
#define UART3_BASEADDR       (u32)(0xbfba5000)
#define UART4_BASEADDR       (u32)(0xbfba6000)
#define MAILBOX_BASEADDR     (u32)(0xbfbd4000)
#define CHIP_CTRLPMU         (u32)(0xbfbd0000)

#endif /* CONFIG_SPL_BUILD */

#define CONSOLE_UART_BASE    UART2_BASEADDR
#define SYS_FREQ             60000000
#endif /* __ASM_ARCH_HARDWARE_H__ */
