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

static int mailbox_ready;
enum remove_echo {
	INITAL_STATE,
	COMMAND_GET,
	COMMAND_READY,
	COMMAND_SEND
};
static enum remove_echo send_echo;
static const char* str_cutting_line = "/*******************************/\n";
static const char* str_exenv_prompt = "# ";

/**
 * getline_async - Read a user input line in servial times. Each time call this
 *		   function, getc() and append line buffer. It will return 1
 *		   until receive Enter/Return key.
 * @line:	Input param, the string received from console.
 * @cancel:	Output param, canceled this line input, new input again.
 *
 * Return: 0: If has not receive Enter/Return key.
 *	   1: If receive an Enter/Return key.
 */
static int getline_async(char **line, int *cancel)
{
	static char buffer[256];
	static int buffer_pos = 0;
	char c;
	*line = buffer;
	*cancel = 0;

	/* If no data avalible in console, return */
	if (tstc() == 0) {
		return 0;
	}

	c = getc();
	switch (c) {
	case '\r':	/* Enter/Return key */
	case '\n':
		puts("\n");
		buffer[buffer_pos++] = '\n';
		buffer[buffer_pos++] = '\0';
		buffer_pos = 0;
		return 1;

	case 0x03:	/* ^C - break */
		buffer_pos = 0;
		*cancel = 1;
		return 0;

	case 0x08:	/* ^H  - backspace */
	case 0x7F:	/* DEL - backspace */
		if (buffer_pos > 0) {
			puts("\b \b");
			buffer_pos--;
		}
		return 0;

	default:
		/* Ignore control characters */
		if (c < 0x20)
			return 0;

		/* Queue up all other characters, but reserve 2 bytes for
		   '\n' and '\0'
		 */
		if (buffer_pos > sizeof(buffer) -2)
			return 0;

		buffer[buffer_pos++] = c;
		printf("%c", c);
		return 0;
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

	if (CSKY_MBOX_DEV_ID_SEND != CSKY_MBOX_DEV_ID_RECV) {
		ret = mailbox_csky_open(CSKY_MBOX_DEV_ID_RECV);
		if (ret != 0) {
			printf("Can't open mailbox-%d, ret=%d\n",
				CSKY_MBOX_DEV_ID_RECV, ret);
			goto out0;
		}
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
			} else if (sent_once < 0) {
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
			} else if (read_once < 0) {
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
	} else {
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

static int cmd_mailbox_csky_shell(void)
{
	char *line;
	char rx_data[64];
	int send_len, sent_len;
	int sent_once, read_once;
	int readline_done = 0;
	int readline_canceled = 1;
	int last_time = 0;
	int present_time;
	int ret;

	ret = mailbox_csky_init();
	if (ret != 0) {
		printf("mailbox init failed, ret=%d", ret);
		return ret;
	}

	puts(str_cutting_line);
	puts(" * Enter shell environment.\n");
	puts(" * Input \"quit\"+<ENTER> to exit\n");
	puts(str_cutting_line);
	if (!mailbox_ready) {
		puts("Starting Linux...");
	}

	ret = mailbox_csky_open(CSKY_MBOX_DEV_ID_SEND);
	if (ret != 0) {
		printf("Can't open mailbox-%d, ret=%d\n",
			CSKY_MBOX_DEV_ID_SEND, ret);
		return ret;
	}

	if (CSKY_MBOX_DEV_ID_SEND != CSKY_MBOX_DEV_ID_RECV) {
		ret = mailbox_csky_open(CSKY_MBOX_DEV_ID_RECV);
		if (ret != 0) {
			printf("Can't open mailbox-%d, ret=%d.\n",
				CSKY_MBOX_DEV_ID_RECV, ret);
			goto out0;
		}
	}

	while (1) {
		udelay(10000);

		if (readline_canceled && mailbox_ready) {
			puts(str_exenv_prompt);
		} else if (!mailbox_ready){
			if (!last_time) {
				last_time = timer_get_us();
				puts(".");
			} else {
				present_time = timer_get_us();
				if ((present_time - last_time) >= 1000000) {
					puts(".");
					last_time = present_time;
				}
			}
		}

		do {
			int read_once =
				mailbox_csky_recv(CSKY_MBOX_DEV_ID_RECV,
						  rx_data, sizeof(rx_data));
			if (read_once > 0) {
				printf("%s", rx_data);
				if (send_echo == COMMAND_READY && strncmp(rx_data, "#", 1) == 0) {
					send_echo = COMMAND_SEND;
				}
			}
			if (!mailbox_ready) {
				if (!strncmp(rx_data, "Welcome", 7)) {
					mailbox_ready = 1;
					send_echo = COMMAND_GET;
				}
			}
		} while (read_once > 0);

		if (send_echo == COMMAND_SEND) {
			strcpy(line, "stty -echo\n");
			send_echo = INITAL_STATE;
		} else {
			readline_done = getline_async(&line, &readline_canceled);
			if (readline_done == 0) {
				if (readline_canceled) {
					puts("\n");
				}
				if(send_echo != COMMAND_SEND)
					continue;
			}
		}

		send_len = strlen(line);
		if (send_len == 0) {
			continue;
		}

		if (strcmp(line, "quit\n") == 0) {
			break;
		}

		if (strcmp(line, "root\n") == 0) {
			if (send_echo == COMMAND_GET) {
				send_echo = COMMAND_READY;
			}
		}

		//printf("Input command is '%s', length=%d\n", line, send_len);
		sent_len = 0;
		do {
			sent_once = mailbox_csky_send(CSKY_MBOX_DEV_ID_SEND,
						     &(line[sent_len]),
						     send_len - sent_len);
			if (sent_once > 0) {
				sent_len += sent_once;
			} else if (sent_once < 0) {
				printf("Send fail at offset=%u\n", sent_len);
				ret = -EIO;
				goto out1;
			}
		} while (sent_len < send_len);
	}

	puts(str_cutting_line);
	puts(" * Exit shell environment\n");
	puts(str_cutting_line);

out1:
	mailbox_csky_close(CSKY_MBOX_DEV_ID_RECV);
out0:
	if (CSKY_MBOX_DEV_ID_SEND != CSKY_MBOX_DEV_ID_RECV)
		mailbox_csky_close(CSKY_MBOX_DEV_ID_SEND);
	return ret;
}

static int
do_mailbox_csky(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	if (argc == 1 || strcmp(argv[1], "help") == 0) {
		cmd_usage(cmdtp);
		ret = 0;
	} else if (argc == 2 && strcmp(argv[1], "list") == 0) {
		cmd_mailbox_csky_list_names();
	} else if (argc <= 3 && strcmp(argv[1], "test") == 0) {
		cmd_mailbox_csky_selftest((argc == 2) ? NULL : argv[2]);
	} else if (argc == 2 && strcmp(argv[1], "shell") == 0) {
		cmd_mailbox_csky_shell();
	} else {
		ret = CMD_RET_USAGE;
	}

	return ret;
}

#ifdef CONFIG_SYS_LONGHELP
static char mailbox_csky_longhelp_text[] =
		"shell         - Enter Linux shell\n"
	"mailbox test [string] - Auto selftest by Send/Recv string\n"
	"mailbox list          - List all mailbox names\n";
#else
static char mailbox_csky_longhelp_text[] = "";
#endif

U_BOOT_CMD(
	mailbox,	3,	0,	do_mailbox_csky,
	"Do mailbox self loop test or send command and recv response back",
	mailbox_csky_longhelp_text
);
