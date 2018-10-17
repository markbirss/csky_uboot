/*
 * Copyright (C) 2017 C-SKY MicroSystems Co.,Ltd.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ASM_ARCH_HARDWARE_H__
#define __ASM_ARCH_HARDWARE_H__

#ifdef CONFIG_SPL_BUILD
#define INTC_BASEADDR        (u32)(0x1fb15000)
#define GPIOA_BASEADDR       (u32)(0x1fbab000)
#define GPIOB_BASEADDR       (u32)(0x1fbac000)
#define GPIOC_BASEADDR       (u32)(0x1fbad000)
#define GPIOD_BASEADDR       (u32)(0x1fbae000)
#define TIMER_BASEADDR       (u32)(0x1fba2000)
#define GMAC_BASEADDR        (u32)(0x1fb10000)
#define EMMC_BASEADDR        (u32)(0x1fb13000)
#define OM_BASEADDR          (u32)(0x1fbd0100)
#define SPI0_BASEADDR        (u32)(0x1fba8000)
#define UART0_BASEADDR       (u32)(0x1fba4000)
#define UART1_BASEADDR       (u32)(0x1fba5000)

#else

#define INTC_BASEADDR        (u32)(0xbfb15000)
#define GPIOA_BASEADDR       (u32)(0xbfbab000)
#define GPIOB_BASEADDR       (u32)(0xbfbac000)
#define GPIOC_BASEADDR       (u32)(0xbfbad000)
#define GPIOD_BASEADDR       (u32)(0xbfbae000)
#define TIMER_BASEADDR       (u32)(0xbfba2000)
#define GMAC_BASEADDR        (u32)(0xbfb10000)
#define EMMC_BASEADDR        (u32)(0xbfb13000)
#define OM_BASEADDR          (u32)(0xbfbd0100)
#define SPI0_BASEADDR        (u32)(0xbfba8000)
#define UART0_BASEADDR       (u32)(0xbfba4000)
#define UART1_BASEADDR       (u32)(0xbfba5000)

#endif /* CONFIG_SPL_BUILD */

#define CONSOLE_UART_BASE	UART0_BASEADDR
#define CHIP_CTRL_BASEADDR	0x1fbd0000
#define SOFTWARE_RESET_CONTROL	0x38
#define MPU_BASEADDR		0x1fbd0800
#endif /* __ASM_ARCH_HARDWARE_H__ */
