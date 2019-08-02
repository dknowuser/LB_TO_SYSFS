#include <linux/module.h>
#include <linux/platform_device.h>

// Module driver structure

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
