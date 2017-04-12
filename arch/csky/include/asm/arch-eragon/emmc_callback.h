/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __ASM_ARCH_EMMC_CALLBACK_H
#define __ASM_ARCH_EMMC_CALLBACK_H
#include "emmc.h"

/* Definitions for cmd status */
#define CMD_STAT_ABSENT		    0
#define CMD_STAT_STARTED	    1
#define CMD_STATE_READRESP	    2
#define CMD_STATE_READDAT	    3
#define CMD_STATE_WRITEDAT	    4
#define CMD_COMMAND_DONE	    5
#define CMD_COMMAND_ABORTING	6
#define CMD_STATE_POLLD   	    7

emmc_postproc_callback emmc_get_post_callback(uint32_t);
emmc_preproc_callback emmc_get_pre_callback(uint32_t);
void emmc_set_data_trans_params(uint32_t slot, uint8_t *data_buffer,
                                uint32_t num_of_blocks, uint32_t epoch_count,
                                uint32_t flag, uint32_t custom_blocksize);

void emmc_set_current_task_status_t(uint32_t slot, uint32_t *resp_buffer,
                                    CK_UINT8 *data_buffer);

#endif /* __ASM_ARCH_EMMC_CALLBACK_H */

