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
#define ERAGON_INTC_BASE         0xbfba3000  /* The base of intc */
#define ERAGON_GPIOA_BASE        0xbfbac000
#define ERAGON_GPIOB_BASE        0xbfbad000
#define ERAGON_GPIOC_BASE        0xbfbae000
#define ERAGON_GPIOD_BASE        0xbfbaf000
#define ERAGON_TIMER0_BASE       0xbfba2000
#define ERAGON_TIMER1_BASE       0xbfba2014
#define ERAGON_TIMER2_BASE       0xbfba2028
#define ERAGON_GMAC_ADDRBASE     0xbfb10000


#define SYSTEM_CLOCK 60     /* It means SYSTEM_CLOCK (M) */
#define CONFIG_NR_DRAM_BANKS 1
#define CONFIG_BOARD_EARLY_INIT_R
#define CONFIG_SYS_HZ 1000
#define CONFIG_IDENT_STRING "ERAGON 0.5"
#define DEBUG
/* #define CONFIG_SYS_GENERIC_GLOBAL_DATA */
#define CONFIG_DDR_LOAD_ADDR (uint8_t *)(0x19000)
#define CONFIG_JUMP_DDR (uint32_t)(0x19000)

#define ERAGON_MMC0_BASE (uint32_t)(0xbfb13000)
#define ERAGON_MMC
#define CONFIG_DWMMC
#define CONFIG_ERAGON_DWMMC
/* #define CONFIG_SYS_MMC_U_BOOT_START (0x00200000) */
/* #define CONFIG_SYS_MMC_U_BOOT_SIZE (512 << 10) */
/* #define CONFIG_SYS_MMC_U_BOOT_DST (0x00200000) */
/* #define CONFIG_SYS_MMC_U_BOOT_OFFS (12 << 10) */
/* #define CONFIG_SPL_MMC_SUPPORT */
#define CONFIG_GENERIC_MMC

#define HAVE_BLOCK_DEVICE
#define CONFIG_SYS_NO_FLASH
#define CONFIG_ENV_IS_IN_MMC
/* #define CONFIG_ENV_IS_NOWHERE */
#define CONFIG_SYS_MMC_ENV_DEV 0
/* #define CONFIG_SPL_MMC_BOOT */

#define CONFIG_BOUNCE_BUFFER
/* #define CONFIG_ENV_IS_IN_MMC 1 */
/* #define CONFIG_SYS_MMC_ENV_DEV 0 */
#define TIMER_ID 0
#define TIME_IRQ 4

#define CONFIG_SYS_NS16550_MEM32
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_BAUDRATE_TABLE {115200}
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_COM2 0xbfba4000
#define CONFIG_SYS_NS16550_CLK 60000000
#define CONFIG_CONS_INDEX 2
#define CONFIG_SYS_NS16550_REG_SIZE 1

#define CK_INTC_BASEADDRESS 0x1fba3000
#define CROSS_COMPILE csky-abiv2-elf-
#define CONFIG_BAUDRATE   115200
/* #define CONFIG_SPL_FRAMEWORK */
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_UBOOT_SIZE 0x4000
#define CONFIG_SPL_STACK  (CONFIG_SYS_SRAM_BASE + 0x4000 - 0x8)
/* #define CONFIG_SPL_SERIAL_SUPPORT */
/* #define CONFIG_SYS_ONENAND_BASE 0xE7100000 */
#define CK_OM_ADDRBASE    (uint32_t)(0x1fbd0100)
#define CK_SPI0_ADDRBASE  (uint32_t)(0x1fb18000)
#define CK_SPI1_ADDRBASE  (uint32_t)(0x1fbab000)
#define CK_UART0_ADDRBASE (uint32_t)(0x1fb14000)
#define CK_UART1_ADDRBASE (uint32_t)(0x1fb15000)
#define CK_UART2_ADDRBASE (uint32_t)(0x1fba4000)
#define CK_UART3_ADDRBASE (uint32_t)(0x1fba5000)
#define CK_UART4_ADDRBASE (uint32_t)(0x1fba6000)

#define CPU_DEFAULT_FREQ  60000000
#define LSP_DEFAULT_FREQ  60000000
#define HSP_DEFAULT_FREQ  60000000
#define DDR_DEFAULT_FREQ  60000000
#define SRAM_DEFAULT_FREQ 60000000

#define TIMER_DEFAULT_FREQ  LSP_DEFAULT_FREQ
#define UART_DEFAULT_FREQ   CPU_DEFAULT_FREQ
#define SPI_DEFAULT_FREQ    HSP_DEFAULT_FREQ
#define EMMC_DEFAULT_FREQ   HSP_DEFAULT_FREQ


#define CONSOLE_UART_BASE CK_UART2_ADDRBASE
#define TIMEOUT_COUNT_TIMER TIMER0
#define TIMEOUT_VALUE 500
#define SOC_EMMC_ADDRBASE (uint32_t)0x1FB13000

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
#define CONFIG_SYS_TEXT_BASE	0x17a00000
#define CONFIG_SPL_TEXT_BASE    0x1fd00000

/* input clock of PLL (the SMDK2410 has 12MHz input clock) */
#define CONFIG_SYS_CLK_FREQ	60000000
/* #define CONFIG_SYS_TIMER_RATE CONFIG_SYS_CLK_FREQ */

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
#define PHYS_SRAM_1		0x1fd00000
#define PHYS_SDRAM_1            0x0
#define PHYS_SDRAM_1_SIZE	0x18000000 /* 384M */

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
#define CONFIG_ENV_SIZE			0x400
/* allow to overwrite serial and ethaddr */
/* #define CONFIG_ENV_OVERWRITE */

/*
 * Size of malloc() pool
 * BZIP2 / LZO / LZMA need a lot of RAM
 */
#define CONFIG_SYS_MALLOC_LEN	(4 * 1024 * 1024)

/* #define CONFIG_SYS_MONITOR_LEN	(448 * 1024) */
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_TEXT_BASE

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
#define CONFIG_SYS_SRAM_BASE        PHYS_SRAM_1
#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_TEXT_BASE  + 0x80000 - 0x8)
#define CONFIG_BOARD_EARLY_INIT_F

/* #define CONFIG_SYS_LDSCRIPT "arch/csky/cpu/ck807/u-boot.lds" */
/* #define CONFIG_SPL_LDSCRIPT "arch/csky/cpu/ck807/u-boot-spl.lds" */
#define CONFIG_SPL_START_S_PATH "arch/csky/cpu/ck807"

#endif /* __CONFIG_H */
