/*
 * Copyright (C) 2017 C-SKY MicroSystems Co.,Ltd.
 * Author: Charles Lu <chongzhi_lu@c-sky.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <version.h>
#include <linux/compiler.h>
#ifdef CONFIG_SYS_COREBOOT
#include <asm/arch/sysinfo.h>
#endif
#include <asm/mailbox-csky.h>

static const char* str_cutting_line = "/*******************************/\n";
static const char* str_exenv_prompt = "[exe]# ";

static char *getline(void)
{
	static char buffer[100];
	char c;
	size_t i;

	i = 0;
	while (1) {
		buffer[i] = '\0';

		c = getc();

		switch (c) {
		case '\r':	/* Enter/Return key */
		case '\n':
			puts("\n");
			return buffer;

		case 0x03:	/* ^C - break */
			return NULL;

		case 0x5F:
		case 0x08:	/* ^H  - backspace */
		case 0x7F:	/* DEL - backspace */
			if (i) {
				puts("\b \b");
				i--;
			}
			break;

		default:
			/* Ignore control characters */
			if (c < 0x20)
				break;
			/* Queue up all other characters */
			buffer[i++] = c;
			printf("%c", c);
			break;
		}
	}
}

static int cmd_mailbox_csky_list_names(void)
{
	char **names;
	uint count;
	int ret;

	ret = mailbox_csky_get_names(&names, &count);
	if (ret != 0) {
		printf("Get mailbox names failed, ret=%d", ret);
		return ret;
	}

	uint i;
	puts("The mailbox names are:\n");
	for (i = 0; i < count; ++i) {
		printf("\t%s\n", names[i]);
	}
	puts("\n");
	return 0;
}

static int cmd_mailbox_csky_selftest(char *tx_data)
{
	int ret;
	char rx_data[1024];
	uint transfer_len;		/* Need to tx/rx length */
	uint sent_len = 0, read_len = 0;/* Has sent/read length */

	memset(rx_data, 0, sizeof(rx_data));
	ret = mailbox_csky_init();
	if (ret != 0) {
		printf("mailbox init failed, ret=%d", ret);
		return ret;
	}

	if (tx_data == NULL) {
		tx_data = "This is a mailbox test string";
	}

	transfer_len = strlen(tx_data) + 1;	/* "+ 1" for last '\0' */

	ret = mailbox_csky_open(CSKY_MBOX_DEV_ID_SEND);
	if (ret != 0) {
		printf("Can't open mailbox-%d, ret=%d\n",
			CSKY_MBOX_DEV_ID_SEND, ret);
		return ret;
	}

	ret = mailbox_csky_open(CSKY_MBOX_DEV_ID_RECV);
	if (ret != 0) {
		printf("Can't open mailbox-%d, ret=%d\n",
			CSKY_MBOX_DEV_ID_RECV, ret);
		goto out0;
	}

	while (sent_len < transfer_len || read_len < transfer_len) {
		/* Send data via CSKY_MBOX_DEV_ID_SEND */
		if (sent_len < transfer_len) {
			int sent_once =
				mailbox_csky_send(CSKY_MBOX_DEV_ID_SEND,
						  &(tx_data[sent_len]),
						  transfer_len - sent_len);
			if (sent_once > 0) {
				sent_len += sent_once;
			}
			else if (sent_once < 0) {
				printf("Send fail at offset=%u\n", sent_len);
				ret = -EIO;
				goto out1;
			}
		}

		/* Recv data via CSKY_MBOX_DEV_ID_RECV */
		if (read_len < transfer_len) {
			int read_once =
				mailbox_csky_recv(CSKY_MBOX_DEV_ID_RECV,
						  &(rx_data[read_len]),
						  sizeof(rx_data) - read_len);
			if (read_once > 0) {
				read_len += read_once;
			}
			else if (read_once < 0) {
				printf("Send fail at offset=%u\n", sent_len);
				ret = -EIO;
				goto out1;
			}
		}
	}

	rx_data[sizeof(rx_data) - 1] = '\0';
	printf("\nSend data is:'%s'\n", tx_data);
	printf("\nRecv data is:'%s'\n", rx_data);

	printf("\nTransfer len is %d bytes, Sent %u bytes, Recv %u bytes.\n",
		transfer_len, sent_len, read_len);
	if (transfer_len == sent_len && sent_len == read_len &&
	    memcmp(tx_data, rx_data, transfer_len) == 0) {
		ret = 0;
	}
	else {
		ret = -1;
	}
	puts(str_cutting_line);
	printf(" * Mailbox selftest %s\n", (ret == 0) ? "Success" : "Failed");
	puts(str_cutting_line);

out1:
	mailbox_csky_close(CSKY_MBOX_DEV_ID_RECV);
out0:
	mailbox_csky_close(CSKY_MBOX_DEV_ID_SEND);
	return ret;
}

static int cmd_mailbox_csky_exe(void)
{
	char *line;
	puts(str_cutting_line);
	puts(" * Enter execute environment.\n");
	puts(" * Input \"quit\"+<ENTER> to exit\n");
	puts(str_cutting_line);

	while (1) {
		puts(str_exenv_prompt);
		line = getline();
		if (line == NULL) {
			puts("\n");
			continue;
		}
		if (strlen(line) == 0) {
			continue;
		}

		if (strcmp(line, "quit") == 0) {
			break;
		}

		printf("Input command is '%s'\n", line);
	}

	puts(str_cutting_line);
	puts(" * Exit execute environment\n");
	puts(str_cutting_line);
	return 0;
}

static int
do_mailbox_csky(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	if (argc == 1 || strcmp(argv[1], "help") == 0) {
		cmd_usage(cmdtp);
		ret = 0;
	}
	else if (argc == 2 && strcmp(argv[1], "list") == 0) {
		cmd_mailbox_csky_list_names();
	}
	else if (argc <= 3 && strcmp(argv[1], "test") == 0) {
		cmd_mailbox_csky_selftest((argc == 2) ? NULL : argv[2]);
	}
	else if (argc == 2 && strcmp(argv[1], "exe") == 0) {
		cmd_mailbox_csky_exe();
	}
	else {
		ret = CMD_RET_USAGE;
	}

	return ret;
}

#ifdef CONFIG_SYS_LONGHELP
static char mailbox_csky_longhelp_text[] =
	        "exe           - Enter executing environment\n"
	"mailbox test [string] - Auto selftest by Send/Recv string \n"
	"mailbox list          - List all mailbox names\n";
#else
static char mailbox_csky_longhelp_text[] = "";
#endif

U_BOOT_CMD(
	mailbox,	3,	0,	do_mailbox_csky,
	"Do mailbox self loop test or send command and recv response back",
	mailbox_csky_longhelp_text
);
