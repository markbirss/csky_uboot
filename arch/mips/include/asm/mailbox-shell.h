/*
 * Copyright (C) 2017 C-SKY MicroSystems Co.,Ltd.
 * Author: Charles Lu <chongzhi_lu@c-sky.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _MAILBOX_SHELL_H
#define _MAILBOX_SHELL_H

#define SHELL_MBOX_DEV_ID_SEND		CONFIG_MAILBOX_DEV_ID_SEND
#define SHELL_MBOX_DEV_ID_RECV		CONFIG_MAILBOX_DEV_ID_RECV

#define SHELL_MBOX_MAX_MESSAGE_LENGTH	64	/* u32 x 16 */
#define SHELL_MBOX_MSSG_HEAD_LENGTH	4
#define SHELL_MBOX_MAX_DATA_LENGTH	(SHELL_MBOX_MAX_MESSAGE_LENGTH - \
					 SHELL_MBOX_MSSG_HEAD_LENGTH)

int mailbox_shell_get_names(char ***names, uint *count);
int mailbox_shell_init(void);
int mailbox_shell_open(uint dev_id);
int mailbox_shell_close(uint dev_id);
int mailbox_shell_send(uint dev_id, char *data, uint length);
int mailbox_shell_recv(uint dev_id, char *data, uint length);

#endif /* _MAILBOX_SHELL_H */

