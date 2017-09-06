#
# Copyright (C) 2017 C-SKY MicroSystems Co.,Ltd.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y := eragon_evb.o
obj-y += gpio/gpio.o

ifdef CONFIG_SPL_BUILD

ifdef CONFIG_ERAGON_ORIGINAL
obj-y += ddr/ddr_original.o
obj-y += eragon_original/spl.o
endif #CONFIG_ERAGON_ORIGINAL

ifdef CONFIG_ERAGON_LS
obj-y += eragon_ls/spl.o
obj-y += ddr/ddr_ls.o
endif #CONFIG_ERAGON_LS

obj-y += om/om.o

else #CONFIG_SPL_BUILD

ifdef CONFIG_ERAGON_LS
obj-y += mailbox/mailbox-csky.o
endif

endif #CONFIG_SPL_BUILD
