/*
 * Copyright (c) 2014 Google, Inc
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <dm/root.h>
#include <dm/test.h>
#include <dm/ut.h>
#include <dm/util.h>

DECLARE_GLOBAL_DATA_PTR;

static int testbus_drv_probe(struct udevice *dev)
{
	return dm_scan_fdt_node(dev, gd->fdt_blob, dev->of_offset, false);
}

static const struct udevice_id testbus_ids[] = {
	{
		.compatible = "denx,u-boot-test-bus",
		.data = DM_TEST_TYPE_FIRST },
	{ }
};

U_BOOT_DRIVER(testbus_drv) = {
	.name	= "testbus_drv",
	.of_match	= testbus_ids,
	.id	= UCLASS_TEST_BUS,
	.probe	= testbus_drv_probe,
	.priv_auto_alloc_size = sizeof(struct dm_test_priv),
	.platdata_auto_alloc_size = sizeof(struct dm_test_pdata),
};

UCLASS_DRIVER(testbus) = {
	.name		= "testbus",
	.id		= UCLASS_TEST_BUS,
};

/* Test that we can probe for children */
static int dm_test_bus_children(struct dm_test_state *dms)
{
	int num_devices = 4;
	struct udevice *bus;
	struct uclass *uc;

	ut_assertok(uclass_get(UCLASS_TEST_FDT, &uc));
	ut_asserteq(num_devices, list_count_items(&uc->dev_head));

	/* Probe the bus, which should yield 3 more devices */
	ut_assertok(uclass_get_device(UCLASS_TEST_BUS, 0, &bus));
	num_devices += 3;

	ut_assertok(uclass_get(UCLASS_TEST_FDT, &uc));
	ut_asserteq(num_devices, list_count_items(&uc->dev_head));

	ut_assert(!dm_check_devices(dms, num_devices));

	return 0;
}
DM_TEST(dm_test_bus_children, DM_TESTF_SCAN_PDATA | DM_TESTF_SCAN_FDT);

/* Test our functions for accessing children */
static int dm_test_bus_children_funcs(struct dm_test_state *dms)
{
	const void *blob = gd->fdt_blob;
	struct udevice *bus, *dev;
	int node;

	ut_assertok(uclass_get_device(UCLASS_TEST_BUS, 0, &bus));

	/* device_get_child() */
	ut_assertok(device_get_child(bus, 0, &dev));
	ut_asserteq(-ENODEV, device_get_child(bus, 4, &dev));
	ut_assertok(device_get_child_by_seq(bus, 5, &dev));
	ut_assert(dev->flags & DM_FLAG_ACTIVATED);
	ut_asserteq_str("c-test@5", dev->name);

	/* Device with sequence number 0 should be accessible */
	ut_asserteq(-ENODEV, device_find_child_by_seq(bus, -1, true, &dev));
	ut_assertok(device_find_child_by_seq(bus, 0, true, &dev));
	ut_assert(!(dev->flags & DM_FLAG_ACTIVATED));
	ut_asserteq(-ENODEV, device_find_child_by_seq(bus, 0, false, &dev));
	ut_assertok(device_get_child_by_seq(bus, 0, &dev));
	ut_assert(dev->flags & DM_FLAG_ACTIVATED);

	/* There is no device with sequence number 2 */
	ut_asserteq(-ENODEV, device_find_child_by_seq(bus, 2, false, &dev));
	ut_asserteq(-ENODEV, device_find_child_by_seq(bus, 2, true, &dev));
	ut_asserteq(-ENODEV, device_get_child_by_seq(bus, 2, &dev));

	/* Looking for something that is not a child */
	node = fdt_path_offset(blob, "/junk");
	ut_asserteq(-ENODEV, device_find_child_by_of_offset(bus, node, &dev));
	node = fdt_path_offset(blob, "/d-test");
	ut_asserteq(-ENODEV, device_find_child_by_of_offset(bus, node, &dev));

	/* Find a valid child */
	node = fdt_path_offset(blob, "/some-bus/c-test@1");
	ut_assertok(device_find_child_by_of_offset(bus, node, &dev));
	ut_assert(!(dev->flags & DM_FLAG_ACTIVATED));
	ut_assertok(device_get_child_by_of_offset(bus, node, &dev));
	ut_assert(dev->flags & DM_FLAG_ACTIVATED);

	return 0;
}
DM_TEST(dm_test_bus_children_funcs, DM_TESTF_SCAN_PDATA | DM_TESTF_SCAN_FDT);