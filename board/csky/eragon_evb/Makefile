#
# Copyright (C) 2017 C-SKY MicroSystems Co.,Ltd.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y := eragon_evb.o
obj-y += gpio/gpio.o

ifdef CONFIG_SPL_BUILD

ifdef CONFIG_ERAGON_ORIGINAL
ifdef CONFIG_IS_ASIC
obj-y += ddr/ddr_asic.o
else
obj-y += ddr/ddr_original.o
endif #CONFIG_IS_ASIC
obj-y += eragon_original/spl.o
endif #CONFIG_ERAGON_ORIGINAL

ifdef CONFIG_ERAGON2
obj-y += eragon2/spl.o
obj-y += ddr/ddr_ls.o
endif #CONFIG_ERAGON2

obj-y += om/om.o

else #CONFIG_SPL_BUILD

ifdef CONFIG_ERAGON2
obj-y += mailbox/mailbox-csky.o
endif

endif #CONFIG_SPL_BUILD
