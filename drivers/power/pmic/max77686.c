/*
 *  Copyright (C) 2014-2015 Samsung Electronics
 *  Przemyslaw Marczak  <p.marczak@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <fdtdec.h>
#include <errno.h>
#include <dm.h>
#include <i2c.h>
#include <power/pmic.h>
#include <power/regulator.h>
#include <power/max77686_pmic.h>

DECLARE_GLOBAL_DATA_PTR;

static const struct pmic_child_info pmic_childs_info[] = {
	{ .prefix = "ldo", .driver = MAX77686_LDO_DRIVER },
	{ .prefix = "buck", .driver = MAX77686_BUCK_DRIVER },
	{ },
};

static int max77686_write(struct udevice *dev, uint reg, const uint8_t *buff,
			  int len)
{
	if (dm_i2c_write(dev, reg, buff, len)) {
		error("write error to device: %p register: %#x!", dev, reg);
		return -EIO;
	}

	return 0;
}

static int max77686_read(struct udevice *dev, uint reg, uint8_t *buff, int len)
{
	if (dm_i2c_read(dev, reg, buff, len)) {
		error("read error from device: %p register: %#x!", dev, reg);
		return -EIO;
	}

	return 0;
}

static int max77686_bind(struct udevice *dev)
{
	int regulators_node;
	const void *blob = gd->fdt_blob;
	int childs;

	regulators_node = fdt_subnode_offset(blob, dev->of_offset,
					     "voltage-regulators");
	if (regulators_node <= 0) {
		debug("%s: %s regulators subnode not found!", __func__,
							     dev->name);
		return -ENXIO;
	}

	debug("%s: '%s' - found regulators subnode\n", __func__, dev->name);

	childs = pmic_bind_childs(dev, regulators_node, pmic_childs_info);
	if (!childs)
		debug("%s: %s - no child found\n", __func__, dev->name);

	/* Always return success for this device */
	return 0;
}

static struct dm_pmic_ops max77686_ops = {
	.reg_count = MAX77686_NUM_OF_REGS,
	.read = max77686_read,
	.write = max77686_write,
};

static const struct udevice_id max77686_ids[] = {
	{ .compatible = "maxim,max77686" },
	{ }
};

U_BOOT_DRIVER(pmic_max77686) = {
	.name = "max77686 pmic",
	.id = UCLASS_PMIC,
	.of_match = max77686_ids,
	.bind = max77686_bind,
	.ops = &max77686_ops,
};