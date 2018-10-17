#
# Copyright (C) 2017 C-SKY MicroSystems Co.,Ltd.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y := byhl_evb.o
obj-y += gpio/gpio.o

ifdef CONFIG_SPL_BUILD
ifdef CONFIG_BYHL
obj-y += byhl/ddr.o
obj-y += byhl/spl.o
endif #CONFIG_BYHL
obj-y += om/om.o
endif #CONFIG_SPL_BUILD

