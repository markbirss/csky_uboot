#include "configs/eragon.h"

#ifdef CONFIG_CKCPU_MMU
#define TRILOBITE_MMC_SCONR	0xb0001000
#define TRILOBITE_MMC_SMSKR1	0xb0001060
#else
#define TRILOBITE_MMC_SCONR	0x10001000
#define TRILOBITE_MMC_SMSKR1	0x10001060
#endif

.global lowlevel_init

lowlevel_init:
        .text
        .balign 4
