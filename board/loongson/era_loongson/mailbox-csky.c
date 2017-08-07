/*
 * Copyright (C) 2017 C-SKY MicroSystems Co.,Ltd.
 * Author: Charles Lu <chongzhi_lu@c-sky.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/mailbox-csky.h>
#include "mach/timer.h"

//#define DEBUG
#undef DEBUG

#define upalign4(a) (((a) + 3) & 0xfffffffc)	/* For examples: 1->4, 7->8 */

#undef dev_err
#define dev_err(dev, fmt, args...) { \
	printf("\t%s - error: ", (dev) == NULL ? \
	       "mailbox-x" : ((struct mbox_csky_device *)(dev))->name); \
	printf(fmt, ##args); \
}

#undef dev_dbg
#ifdef DEBUG
#define dev_dbg(dev, fmt, args...) { \
	printf("\t%s - debug: ", (dev) == NULL ? \
	       "mailbox-x" : ((struct mbox_csky_device *)(dev))->name); \
	printf(fmt, ##args); \
}
#else
#define dev_dbg(dev, fmt, args...)
#endif

#define CSKY_MBOX_DEV_ID0	0
#define CSKY_MBOX_DEV_ID1	1
#define CSKY_MBOX_DEV_COUNT	2

/* Interrupt Generate Register (R/W)
 * Write 1 into it, enable interrupt to client immediately.
 * Write 0 into it, disable interrupt to client immediately.
 */
#define CSKY_MBOX_INTGR		0x00

/* Interrupt Clear Register (W1C)
 * Write 1 into it, clear the interrupt
 */
#define CSKY_MBOX_INTCR		0x04

/* Interrupt Mask Register (R/W)
 * 1 means mask interrupt bit
 * 0 means unmask interrupt bit
 */
#define CSKY_MBOX_INTMR		0x08

/* Interrupt Register Status Register (RO)
 * Read 1 means has interrupt
 * Read 0 means no interrupt
 */
#define CSKY_MBOX_INTRSR	0x0C

/* Interrupt Masked Status Register (RO)
 * Read 1 means has interrupt on masked bit
 * Read 0 means no interrupt on masked bit
 */
#define CSKY_MBOX_INTMSR	0x10

/* Interrupt Enable (R/W)
 * 1 means interrupt enabled
 * 0 means interrupt disabled
 */
#define CSKY_MBOX_INTENB	0x14


/* 0x18 is register length from CSKY_MBOX_INTGR to CSKY_MBOX_INTENB */
#define MBOX_INTGR_ADDR(mbox)	\
	(mbox->base + 0x18*(mbox->dev_id ? 0 : 1) + CSKY_MBOX_INTGR)
#define MBOX_INTCR_ADDR(mbox)	\
	(mbox->base + 0x18*(mbox->dev_id) + CSKY_MBOX_INTCR)
#define MBOX_INTMR_ADDR(mbox)	\
	(mbox->base + 0x18*(mbox->dev_id) + CSKY_MBOX_INTMR)
#define MBOX_INTRSR_ADDR(mbox)	\
	(mbox->base + 0x18*(mbox->dev_id) + CSKY_MBOX_INTRSR)
#define MBOX_INTMSR_ADDR(mbox)	\
	(mbox->base + 0x18*(mbox->dev_id) + CSKY_MBOX_INTMSR)
#define MBOX_INTENB_ADDR(mbox)	\
	(mbox->base + 0x18*(mbox->dev_id) + CSKY_MBOX_INTENB)
#define MBOX_TX_MSSG_ADDR(mbox) (mbox->base + 0x18*2 + (mbox->dev_id ? 64 : 0))
#define MBOX_RX_MSSG_ADDR(mbox) (mbox->base + 0x18*2 + (mbox->dev_id ? 0 : 64))

#define TX_GENERATE_INTERRUPT(mbox)	writel(1, MBOX_INTGR_ADDR(mbox))
#define TX_READ_INTERRUPT(mbox)		readl(MBOX_INTGR_ADDR(mbox))
#define RX_CLEAR_INTERRUPT(mbox) 	writel(1, MBOX_INTCR_ADDR(mbox))
#define RX_MASK_INTERRUPT(mbox) 	writel(1, MBOX_INTMR_ADDR(mbox))
#define RX_UNMASK_INTERRUPT(mbox)	writel(0, MBOX_INTMR_ADDR(mbox))
#define RX_READ_INTERRUPT(mbox)		readl(MBOX_INTRSR_ADDR(mbox))
#define RX_READ_MASKED_INTERRUPT(mbox)	readl(MBOX_INTMSR_ADDR(mbox))
#define RX_ENABLE_INTERRUPT(mbox)	writel(1, MBOX_INTENB_ADDR(mbox))
#define RX_DISABLE_INTERRUPT(mbox)	writel(0, MBOX_INTENB_ADDR(mbox))

enum mbox_csky_mssg_type {
	CSKY_MBOX_MSSG_DATA = 'd',	/* Data to receiver */
	CSKY_MBOX_MSSG_ACK  = 'a',	/* ACK to sender */
};


/**
 * struct mbox_message - Description of a message that send to mailbox
 * @mssg_type:	The message type that transfer, refer to mbox_csky_mssg_type
 * @length:	Then data length, must <= CSKY_MBOX_MAX_DATA_LENGTH
 * @reserved0:	Undefined
 * @reserved1:	Undefined
 * @data:	The transfer data. Ignore if mssg_type is CSKY_MBOX_MSSG_ACK
 */
struct mbox_csky_message {
	u8 mssg_type;
	u8 length;
	u8 reserved0;
	u8 reserved1;
	u8 data[CSKY_MBOX_MAX_DATA_LENGTH];
};

/**
 * struct mbox_csky_device - C-SKY mailbox device internal informations
 * @name:	Name of device
 * @dev_id:	Device ID
 * @base:	Base address of this device
 */
struct mbox_csky_device {
	const char	*name;
	uint		dev_id;
	void 		*base;
	bool		is_open;
};

static struct mbox_csky_device mboxes[CSKY_MBOX_DEV_COUNT];
static const char* device_names[CSKY_MBOX_DEV_COUNT] = {
	"mailbox-0", "mailbox-1"
};

static int mbox_get_closed_dev(struct mbox_csky_device **mbox, uint dev_id)
{
	*mbox = NULL;
	if (dev_id != CSKY_MBOX_DEV_ID0 && dev_id != CSKY_MBOX_DEV_ID1) {
		dev_err(*mbox, "Get failed, invalid dev_id(%u)\n", dev_id);
		return -ENODEV;
	}

	if (mboxes[dev_id].base == NULL) {
		dev_err(*mbox, "Get failed, device-%d's address is invalid\n",
			dev_id);
		return -EFAULT;
	}

	if (mboxes[dev_id].is_open) {
		dev_err(*mbox, "Get failed, mailbox(%u) has been opened\n",
			dev_id);
		return -EBUSY;
	}

	*mbox = &(mboxes[dev_id]);
	return 0;
}

static int mbox_get_opened_dev(struct mbox_csky_device **mbox, uint dev_id)
{
	*mbox = NULL;
	if (dev_id != CSKY_MBOX_DEV_ID0 && dev_id != CSKY_MBOX_DEV_ID1) {
		dev_err(*mbox, "Get failed, invalid dev_id(%u)\n", dev_id);
		return -ENODEV;
	}

	if (mboxes[dev_id].base == NULL) {
		dev_err(*mbox, "Get failed, device-%d's address is invalid\n",
			dev_id);
		return -EFAULT;
	}

	if (!mboxes[dev_id].is_open) {
		dev_err(*mbox, "Get failed, mailbox(%u) has been opened\n",
			dev_id);
		return -EBUSY;
	}

	*mbox = &(mboxes[dev_id]);
	return 0;
}

int mailbox_csky_get_names(char ***names, uint *count)
{
	if (names == NULL || count == NULL) {
		dev_err(NULL, "Parameter error\n");
		return -EINVAL;
	}

	*names = (char **)device_names;
	*count = CSKY_MBOX_DEV_COUNT;
	return 0;
}

int mailbox_csky_init(void)
{
	int i;
	for (i = 0; i < CSKY_MBOX_DEV_COUNT; ++i) {
		mboxes[i].name = device_names[i];
		mboxes[i].base = (void*)MAILBOX_BASEADDR;
		mboxes[i].dev_id = i;
		mboxes[i].is_open = false;
	}
	return 0;
}

int mailbox_csky_open(uint dev_id)
{
	struct mbox_csky_device *mbox = NULL;
	int i, ret;

	ret = mbox_get_closed_dev(&mbox, dev_id);
	if (ret != 0) {
		return ret;
	}

	RX_ENABLE_INTERRUPT(mbox);
	RX_UNMASK_INTERRUPT(mbox);
	RX_CLEAR_INTERRUPT(mbox);

	/* Clear RX buffer */
	for(i = 0; i < CSKY_MBOX_MAX_MESSAGE_LENGTH / sizeof(uint); ++i) {
		writel(0, (MBOX_RX_MSSG_ADDR(mbox) + i*4));
	}

	mbox->is_open = true;

	dev_dbg(mbox, "Mailbox(%d) has been opened\n", mbox->dev_id);
	return 0;
}

int mailbox_csky_close(uint dev_id)
{
	struct mbox_csky_device *mbox = NULL;
	int ret;

	ret = mbox_get_opened_dev(&mbox, dev_id);
	if (ret != 0) {
		return ret;
	}

	RX_DISABLE_INTERRUPT(mbox);
	RX_MASK_INTERRUPT(mbox);
	RX_CLEAR_INTERRUPT(mbox);
	mbox->is_open = false;

	dev_dbg(mbox, "Mailbox(%d) has been closed\n", mbox->dev_id);
	return 0;
}

int mailbox_csky_send(uint dev_id, char *data, uint length)
{
	struct mbox_csky_device *mbox = NULL;
	struct mbox_csky_message mssg;
	uint tx_len, copy_len;
	uint mssg_total_len;
	int ret;

	ret = mbox_get_opened_dev(&mbox, dev_id);
	if (ret != 0) {
		return ret;
	}

	if (length == 0) {
		dev_dbg(mbox, "Send data length is 0\n");
		return 0;
	}

	/* If previous tx data has not been received yet, return 0 */
	if (TX_READ_INTERRUPT(mbox)) {
		dev_dbg(mbox, "Previous TX data has not been received yet\n");
		return 0;
	}

	/* Prepare all the tx data into mssg */
	tx_len = min(length, (uint)CSKY_MBOX_MAX_DATA_LENGTH);
	mssg.mssg_type = CSKY_MBOX_MSSG_DATA;
	mssg.length = tx_len;
	mssg.reserved0 = 0;
	mssg.reserved1 = 0;
	mssg_total_len = CSKY_MBOX_MSSG_HEAD_LENGTH + mssg.length;
	memcpy(mssg.data, data, tx_len);

#ifdef DEBUG
	char * bytes = (char *)(&mssg);
	uint dump_len = min(mssg_total_len, (uint)8);
	int i;
	dev_dbg(mbox, "Send mssg %02d bytes%s: ", mssg_total_len,
		(mssg_total_len <= 8) ? "":", first 8");
	for (i = 0; i < dump_len; ++i) {
		printf("%02x ", bytes[i]);
	}
	puts("\n");
#endif

	/* Copy all the whole mssg into device tx registers */
	copy_len = upalign4(mssg_total_len);
	memcpy(MBOX_TX_MSSG_ADDR(mbox), &mssg, copy_len);

	/* Generate tx interrupt for other side receive */
	TX_GENERATE_INTERRUPT(mbox);
	return tx_len;
}

int mailbox_csky_recv(uint dev_id, char *data, uint length)
{
	struct mbox_csky_message mssg;
	struct mbox_csky_device *mbox = NULL;
	uint mssg_total_len;
	int ret;

	/* The max recv data length is 60B, but for input argument, we'd like
	 * length >= 61B for last '\0'. At last, length >= 64B.
	 */
	if (length < CSKY_MBOX_MAX_MESSAGE_LENGTH) {
		dev_err(mbox, "Recv buffer length should>=%d Bytes\n", length);
		return EINVAL;
	}

	ret = mbox_get_opened_dev(&mbox, dev_id);
	if (ret != 0) {
		return ret;
	}

	/* Check whether recv interrupt */
	if (RX_READ_INTERRUPT(mbox) == 0) {
		return 0;
	}

	memcpy(&mssg, MBOX_RX_MSSG_ADDR(mbox), CSKY_MBOX_MAX_MESSAGE_LENGTH);
	RX_CLEAR_INTERRUPT(mbox);

	if (mssg.mssg_type == CSKY_MBOX_MSSG_DATA &&
	    mssg.length <= CSKY_MBOX_MAX_DATA_LENGTH) {
		/* Copy from mssg.data to data */
		memcpy(data, mssg.data, mssg.length);
		data[mssg.length] = '\0';  /* Add '\0' to avoid print error */
		mssg_total_len = CSKY_MBOX_MSSG_HEAD_LENGTH + mssg.length;

		/* Send ACK and generate TX interrupt */
		struct mbox_csky_message *mssg_tx =
			(struct mbox_csky_message*)MBOX_TX_MSSG_ADDR(mbox);
		mssg_tx->mssg_type = CSKY_MBOX_MSSG_ACK;
		TX_GENERATE_INTERRUPT(mbox);

#ifdef DEBUG
		char *bytes = (char *)(&mssg);
		uint dump_len = min(mssg_total_len, (uint)8);
		int i;
		dev_dbg(mbox, "Recv mssg %02d bytes%s: ", mssg_total_len,
			(mssg_total_len <= 8) ? "":", first 8");
		for (i = 0; i < dump_len; ++i) {
			printf("%02x ", bytes[i]);
		}
		puts("\n");
#endif
		return mssg.length;
	} else if (mssg.mssg_type == CSKY_MBOX_MSSG_ACK) {
		dev_dbg(mbox, "ACK received\n");
		return 0;
	} else {
		dev_err(mbox, "Undefined mssg_type:%02x\n", mssg.mssg_type);
		return -ENOMSG;
	}
}

