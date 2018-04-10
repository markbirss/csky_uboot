/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ERAGON_H
#define __ERAGON_H


/*
 * LSP : Timer UART
 * HSP : SPI0 EMMC
 */
#define CPU_DEFAULT_FREQ  60000000
#define LSP_DEFAULT_FREQ  60000000
#define HSP_DEFAULT_FREQ  60000000

/*
 * For the first version
 */
#define CONFIG_CK860 1
#define UBOOT_INTERNAL_VERSION "0.5"
#define CONFIG_SYS_CLK_FREQ CPU_DEFAULT_FREQ
#define CONFIG_BOARD_MMC_SUPPORT
#define CONFIG_BOARD_CONSOLE_SUPPORT
#define CONFIG_BOARD_SPIFLASH_SUPPORT
#define CONFIG_BOARD_PRINTF_SUPPORT
#define CONFIG_SYS_BOOTM_LEN 0x2000000
#define LSP_CLOCK  (LSP_DEFAULT_FREQ / 1000000)    /* It means LSP_CLOCK (M) for timer */
#define CONFIG_NR_DRAM_BANKS 1
#define CONFIG_BOARD_EARLY_INIT_R
#define CONFIG_SYS_HZ 1000
#define CONFIG_IDENT_STRING "ANOLE 0.5"
#define DEBUG
/* #define CONFIG_SYS_GENERIC_GLOBAL_DATA */
#define CONFIG_DDR_LOAD_ADDR (uint8_t *)(0x17a00000)
#define CONFIG_JUMP_DDR (uint32_t)(0x17a00000)

#define ERAGON_MMC0_BASE (uint32_t)(0xbfb13000)
/*#define ERAGON_MMC0_BASE (uint32_t)(0x1fb13000)*/
#define ERAGON_MMC
#define CONFIG_DWMMC
#define CONFIG_ERAGON_DWMMC
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
/* #define CONFIG_SYS_NS16550_COM2 0x1fba4000 */
#define CONFIG_SYS_NS16550_CLK LSP_DEFAULT_FREQ
#define CONFIG_CONS_INDEX 2
#define CONFIG_SYS_NS16550_REG_SIZE 1

#define INTC_BASEADDRESS 0x1fba3000
#define CROSS_COMPILE csky-abiv2-elf-
#define CONFIG_BAUDRATE   115200
/* #define CONFIG_SPL_FRAMEWORK */
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_UBOOT_SIZE 0x28000
#define CONFIG_SPL_STACK  (CONFIG_SYS_SRAM_BASE + 0x8000 - 0x8)
/* #define CONFIG_SPL_SERIAL_SUPPORT */
/* #define CONFIG_SYS_ONENAND_BASE 0xE7100000 */

#define TIMEOUT_COUNT_TIMER TIMER0
#define TIMEOUT_VALUE 500

#define CONFIG_SPL_MAX_FOOTPRINT 0x4000

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ANOLE_SERIAL

#define CONFIG_ANOLE       /* This is a CSKY ANOLE SoC */

/* #define CONFIG_SYS_TEXT_BASE	0x17a00000 */
#define CONFIG_SYS_TEXT_BASE	0x17800000
#define CONFIG_SPL_TEXT_BASE    0x1fc10000

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
#define CONFIG_DOS_PARTITION

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
#define CONFIG_SYS_LONGHELP	/* undef to save memory */
#define CONFIG_AUTO_COMPLETE	/* add autocompletion support */
#define CONFIG_SYS_CBSIZE	256   /* This is Console Buffer size */
/* Print Buffer Size */
/* #define CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE + \
				sizeof(CONFIG_SYS_PROMPT)+16)
 */
#define CONFIG_SYS_MAXARGS	16
/* #define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE */

/* #define CONFIG_DISPLAY_CPUINFO	*/			/* Display cpu info */

#define CONFIG_SYS_MEMTEST_START	0x30000000	/* memtest works on */
#define CONFIG_SYS_MEMTEST_END		0x33F00000	/* 63 MB in DRAM */

#define CONFIG_SYS_LOAD_ADDR		0x80000000

/* support additional compression methods */
/* #define CONFIG_BZIP2 */
/* #define CONFIG_LZO */
/* #define CONFIG_LZMA */

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define PHYS_SRAM_1		0x1fc10000
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
#define CONFIG_ENV_OFFSET 0x10000
/* #define CONFIG_ENV_IS_IN_FLASH */
#define CONFIG_ENV_SIZE			0x2000
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

#ifdef CONFIG_IS_ASIC
#define CONFIG_DTB_NAME "tftpboot ${dtb_load_addr_virt} anole_evb.dtb ; "
#else
#define CONFIG_DTB_NAME "tftpboot ${dtb_load_addr_virt} anole.dtb ; "
#endif

#define CONFIG_EXTRA_ENV_SETTINGS \
	"dtb_start_sector=0x1000\0"   /* dtb start sector -> 2MB */ \
	"dtb_size_sectors=0x1000\0"   /* dtb size in sectors -> 2MB */ \
	"linux_start_sector=0x2000\0" /* linux start sector -> 4MB */  \
	"linux_size_sectors=0xA000\0" /* linux size in sectors -> 20MB */ \
	"dtb_load_addr_virt=0x8f000000\0" \
	"dtb_load_addr_phys=0x0f000000\0"  \
	"linux_load_addr_virt=0x90000000\0"  \
	"linux_load_addr_phys=0x10000000\0" \
	"update_dtb=" \
		CONFIG_DTB_NAME \
		"setexpr fw_sz ${filesize} / 0x200 ; " \
		"setexpr fw_sz ${fw_sz} + 1 ; " \
		"mmc write ${dtb_load_addr_phys} ${dtb_start_sector} ${fw_sz} ; " \
		"setenv dtb_size_sectors ${fw_sz} ; " \
		"saveenv ; " \
		"\0" \
	"update_linux=" \
		"tftpboot ${linux_load_addr_virt} uImage ; " \
		"setexpr fw_sz ${filesize} / 0x200 ; " \
		"setexpr fw_sz ${fw_sz} + 1 ; " \
		"mmc write ${linux_load_addr_phys} ${linux_start_sector} ${fw_sz} ; " \
		"setenv linux_size_sectors ${fw_sz} ; " \
		"saveenv ; " \
		"\0"

#define CONFIG_BOOTCOMMAND \
		"mmc read ${dtb_load_addr_phys} ${dtb_start_sector} ${dtb_size_sectors} ; " \
		"mmc read ${linux_load_addr_phys} ${linux_start_sector} ${linux_size_sectors} ; " \
		"bootm ${linux_load_addr_virt} "

#define CONFIG_SPL_LDSCRIPT "board/csky/anole_evb/anole/u-boot-spl.lds"

#endif /* __ERAGON_H */
