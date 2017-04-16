#
# Copyright (C) 2017 C-SKY Microsystems
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y += board_common.o

ifdef CONFIG_SPL_BUILD
ifdef CONFIG_BOARD_MMC_SUPPORT
obj-y += mmc/emmc.o
obj-y += mmc/emmc_interface.o
obj-y += mmc/emmc_callback.o
endif
obj-$(CONFIG_BOARD_CONSOLE_SUPPORT) += uart/uart.o
obj-$(CONFIG_BOARD_PRINTF_SUPPORT) += mini_printf.o
endif
