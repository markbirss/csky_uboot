/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Gary Jennejohn <garyj@denx.de>
 * David Mueller <d.mueller@elsoft.ch>
 *
 * Configuation settings for the SAMSUNG SMDK2410 board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * For the first version
 */
#define __HAVE_ARCH_MEMCPY 1
#define __HAVE_ARCH_MEMSET 1
#define CROSS_COMPILE csky-abiv2-elf-
/* #define CONFIG_SPL_BUILD 1 */
#define CONFIG_SPL_SERIAL_SUPPORT 1
#define CONFIG_SYS_ONENAND_BASE 0xE7100000
/*#define CONFIG_USE_TINY_PRINTF 1 */
#define CK_OM_ADDRBASE    (uint32_t)(0x1fbd0100)
#define CK_SPI0_ADDRBASE  (uint32_t)(0x1fb18000)
#define CK_SPI1_ADDRBASE  (uint32_t)(0x1fbab000)
#define CK_UART0_ADDRBASE (uint32_t)(0x1fb14000)
#define CK_UART1_ADDRBASE (uint32_t)(0x1fb15000)
#define CK_UART2_ADDRBASE (uint32_t)(0x1fba4000)
#define CK_UART3_ADDRBASE (uint32_t)(0x1fba5000)
#define CK_UART4_ADDRBASE (uint32_t)(0x1fba6000)

#define APB_DEFAULT_FREQ 50000000

#define CONSOLE_UART_BASE CK_UART2_ADDRBASE
#define TIMEOUT_COUNT_TIMER TIMER0
#define TIMEOUT_VALUE 500
#define CK_EMMC_ADDRBASE (uint32_t)0x1FB13000

#define CONFIG_SPL_MAX_FOOTPRINT 0x4000

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ERAGON_SERIAL

#define CONFIG_ERAGON       /* This is a SAMSUNG S3C24x0-type SoC */
#define CONFIG_ERAGON_EVB   /* on a SAMSUNG SMDK2410 Board */

/* #define CONFIG_SYS_TEXT_BASE	0x1fd00000 */
/* #define CONFIG_SYS_TEXT_BASE	0x0000     */
#define CONFIG_SYS_TEXT_BASE	0x00
#define CONFIG_SPL_TEXT_BASE    0x1fd00000

/* input clock of PLL (the SMDK2410 has 12MHz input clock) */
#define CONFIG_SYS_CLK_FREQ	12000000

/* #define CONFIG_SETUP_MEMORY_TAGS */
/* #define CONFIG_INITRD_TAG */

/*
 * Hardware drivers
 */

/************************************************************
 * USB support (currently only works with D-cache off)
 ************************************************************/
/* #define CONFIG_USB_OHCI */
/* #define CONFIG_USB_OHCI_S3C24XX */
/* #define CONFIG_USB_KEYBOARD */
/* #define CONFIG_USB_STORAGE */
/* #define CONFIG_DOS_PARTITION */

/************************************************************
 * RTC
 ************************************************************/
/* #define CONFIG_RTC_ERAGON */

#define CONFIG_BAUDRATE		115200

/*
 * BOOTP options
 */
/* #define CONFIG_BOOTP_BOOTFILESIZE */
/* #define CONFIG_BOOTP_BOOTPATH */
/* #define CONFIG_BOOTP_GATEWAY */
/* #define CONFIG_BOOTP_HOSTNAME */

/*
 * Command line configuration.
 */

/* #define CONFIG_CMDLINE_EDITING */

/* autoboot */
/* #define CONFIG_BOOT_RETRY_TIME	-1 */
/* #define CONFIG_RESET_TO_RETRY */

/* #define CONFIG_NETMASK		255.255.255.0 */
/* #define CONFIG_IPADDR		10.0.0.110 */
/* #define CONFIG_SERVERIP		10.0.0.1 */

/* #if defined(CONFIG_CMD_KGDB) */
/* #define CONFIG_KGDB_BAUDRATE	115200	 */ /* speed to run kgdb serial port */

/*
 * Miscellaneous configurable options
 */
/* #define CONFIG_SYS_LONGHELP	*/	/* undef to save memory */
#define CONFIG_SYS_CBSIZE	256
/* Print Buffer Size */
/* #define CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE + \
				sizeof(CONFIG_SYS_PROMPT)+16)
 */
#define CONFIG_SYS_MAXARGS	16
/* #define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE */

/* #define CONFIG_DISPLAY_CPUINFO	*/			/* Display cpu info */

#define CONFIG_SYS_MEMTEST_START	0x30000000	/* memtest works on */
#define CONFIG_SYS_MEMTEST_END		0x33F00000	/* 63 MB in DRAM */

#define CONFIG_SYS_LOAD_ADDR		0x30800000

/* support additional compression methods */
/* #define CONFIG_BZIP2 */
/* #define CONFIG_LZO */
/* #define CONFIG_LZMA */

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
/* #define CONFIG_NR_DRAM_BANKS	1  */        /* we have 1 bank of DRAM */
/* #define PHYS_SDRAM_1		0x30000000  */ /* SDRAM Bank #1 */
/* #define PHYS_SDRAM_1_SIZE	0x04000000 */  /* 64 MB */

#define PHYS_FLASH_1		0x00000000 /* Flash Bank #0 */

#define CONFIG_SYS_FLASH_BASE	PHYS_FLASH_1

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

/* #define CONFIG_SYS_FLASH_CFI */
/* #define CONFIG_FLASH_CFI_DRIVER */
/* #define CONFIG_FLASH_CFI_LEGACY */
/* #define CONFIG_SYS_FLASH_LEGACY_512Kx16 */
/* #define CONFIG_FLASH_SHOW_PROGRESS	45 */

#define CONFIG_SYS_MAX_FLASH_BANKS	1
/* #define CONFIG_SYS_FLASH_BANKS_LIST     { CONFIG_SYS_FLASH_BASE } */
/* #define CONFIG_SYS_MAX_FLASH_SECT	(19) */

#define CONFIG_ENV_ADDR			(CONFIG_SYS_FLASH_BASE + 0x070000)
/* #define CONFIG_ENV_IS_IN_FLASH */
#define CONFIG_ENV_SIZE			0x10000
/* allow to overwrite serial and ethaddr */
/* #define CONFIG_ENV_OVERWRITE */

/*
 * Size of malloc() pool
 * BZIP2 / LZO / LZMA need a lot of RAM
 */
#define CONFIG_SYS_MALLOC_LEN	(4 * 1024 * 1024)

/* #define CONFIG_SYS_MONITOR_LEN	(448 * 1024) */
/* #define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_FLASH_BASE */

/*
 * NAND configuration
 */
#ifdef CONFIG_CMD_NAND
#define CONFIG_NAND_ERAGON
#define CONFIG_SYS_ERAGON_NAND_HWECC
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0x4E000000
#endif

/* additions for new relocation code, must be added to all boards */
/* #define CONFIG_SYS_SDRAM_BASE	PHYS_SDRAM_1 */
/* #define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_SDRAM_BASE + 0x1000 - \
				GENERATED_GBL_DATA_SIZE)
 */
/* #define CONFIG_BOARD_EARLY_INIT_F */

/* #define CONFIG_SYS_LDSCRIPT "arch/csky/cpu/ck807/u-boot.lds" */
/* #define CONFIG_SPL_LDSCRIPT "arch/csky/cpu/ck807/u-boot-spl.lds" */
#define CONFIG_SPL_START_S_PATH "arch/csky/cpu/ck807"

#endif /* __CONFIG_H */
