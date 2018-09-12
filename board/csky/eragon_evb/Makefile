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
obj-y += eragon_original/ddr_asic.o
else
obj-y += eragon_original/ddr.o
endif #CONFIG_IS_ASIC
obj-y += eragon_original/spl.o
endif #CONFIG_ERAGON_ORIGINAL

ifdef CONFIG_ERAGON2
obj-y += eragon2/spl.o
ifdef CONFIG_IS_ASIC
obj-y += eragon2/ddr_asic.o
else
obj-y += eragon2/ddr.o
endif #CONFIG_IS_ASIC
endif #CONFIG_ERAGON2

obj-y += om/om.o
endif #CONFIG_SPL_BUILD

ifdef CONFIG_FREQ_CHANGE
ifdef CONFIG_IS_ASIC
obj-y += clock/clock.o
obj-y += clock/clock_internal.o
obj-y += clock/cpu_init.o
ifndef CONFIG_SPL_BUILD
obj-$(CONFIG_CMD_CLOCK) += clock/cmd_cpu_frequency.o
endif
endif
endif
