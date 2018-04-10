#
# Copyright (C) 2017 C-SKY MicroSystems Co.,Ltd.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y := anole_evb.o
obj-y += gpio/gpio.o

ifdef CONFIG_SPL_BUILD
ifdef CONFIG_ANOLE
obj-y += anole/ddr.o
obj-y += anole/spl.o
endif #CONFIG_ANOLE
obj-y += om/om.o
endif #CONFIG_SPL_BUILD

