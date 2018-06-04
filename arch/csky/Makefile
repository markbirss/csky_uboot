#
# Copyright (C) 2017 C-SKY Microsystems
#
# SPDX-License-Identifier:	GPL-2.0+
#

cpuflags-$(CONFIG_CK860)	:= -mcpu=ck860

PLATFORM_CPPFLAGS += $(cpuflags-y)


head-y := arch/csky/cpu/$(CPU)/start.o
libs-y += arch/csky/cpu/
libs-y += arch/csky/cpu/$(CPU)/
libs-y += arch/csky/lib/

