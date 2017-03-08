#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y := eragon_evb.o
obj-y += uart/uart.o
obj-y += gpio/gpio.o
ifdef CONFIG_SPL_BUILD
obj-y += mini_printf.o
obj-y += ddr/ddr.o
obj-y += spl.o
obj-y += emmc/emmc.o
obj-y += emmc/emmc_callback.o
obj-y += emmc/emmc_interface.o
obj-y += om/om.o
endif
#obj-y   += spi/spi.o
#obj-y   += spiflash/spiflash.o
#obj-y   += string/string.o
