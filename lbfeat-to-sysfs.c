#include <linux/module.h>
#include <linux/platform_device.h>

// Module device table
static const struct platform_device_id lb_sysfs_ids[] = {
	{ "lbfeat-to-sysfs", 0 },
	{ }
};

static int lb_sysfs_probe(struct platform_device *device)
{
	return 0;
};

static int lb_sysfs_remove(struct platform_device *device)
{
	return 0;
};

// Module driver structure
static struct platform_driver lb_sysfs_driver = {
	.driver = {
		.name = "lbfeat-to-sysfs",
	},
	.probe = lb_sysfs_probe,
	.remove = lb_sysfs_remove,
	.id_table = lb_sysfs_ids,
};

//------------------------------------------------------------------------------
// Module init and exit functions
static int __init lb_sysfs_init(void)
{
	printk(KERN_INFO "Smart LB to sysfs module starts\n");
	return 0;
};

static void __exit lb_sysfs_exit(void)
{
	printk(KERN_INFO "Smart LB to sysfs module exits\n");
};

module_init(lb_sysfs_init);
module_exit(lb_sysfs_exit);

MODULE_LICENSE("Dual BSD/GPL");
//MODULE_AUTHOR("Marchuk Liliya");
MODULE_DESCRIPTION("Module for exporting Smart Loopback management in sysfs");
