/*
 * Copyright (C) 2017 C-SKY MicroSystems Co.,Ltd.
 * Author: Charles Lu <chongzhi_lu@c-sky.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _MAILBOX_CSKY_H
#define _MAILBOX_CSKY_H

#define CSKY_MBOX_DEV_ID_SEND		CONFIG_MAILBOX_DEV_ID_SEND
#define CSKY_MBOX_DEV_ID_RECV		CONFIG_MAILBOX_DEV_ID_RECV

#define CSKY_MBOX_MAX_MESSAGE_LENGTH	64	/* u32 x 16 */
#define CSKY_MBOX_MSSG_HEAD_LENGTH	4
#define CSKY_MBOX_MAX_DATA_LENGTH	(CSKY_MBOX_MAX_MESSAGE_LENGTH - \
					 CSKY_MBOX_MSSG_HEAD_LENGTH)

int mailbox_csky_get_names(char ***names, uint *count);
int mailbox_csky_init(void);
int mailbox_csky_open(uint dev_id);
int mailbox_csky_close(uint dev_id);
int mailbox_csky_send(uint dev_id, char *data, uint length);
int mailbox_csky_recv(uint dev_id, char *data, uint length);

#endif /* _MAILBOX_CSKY_H */

