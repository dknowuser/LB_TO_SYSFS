#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>

// Module device table
static const struct of_device_id lb_sysfs_ids[] = {
	{/* "lbfeat-to-sysfs"*/ .compatible = "lattice,ecp5-hwmon", },
	{ },
};

// TODO: figure out device name
// Uncomment it to enable autoloading
// MODULE_DEVICE_TABLE(of, lb_sysfs_ids);

// Function for plugging Smart Loopback FPGA feature into sysfs
static int lb_sysfs_probe(struct platform_device *pdev)
{
	printk(KERN_INFO "Smart LB to sysfs module has been probed\n");
	return 0;
};

// Function for unplugging Smart Loopback FPGA feature from sysfs
static int lb_sysfs_remove(struct platform_device *pdev)
{
	return 0;
};

// Module driver structure
static struct platform_driver lb_sysfs_driver = {
	.driver = {
		.name = "lbfeat-to-sysfs",
		.of_match_table = of_match_ptr(lb_sysfs_ids),
	},
	.probe = lb_sysfs_probe,
	.remove = lb_sysfs_remove,
	//.id_table = lb_sysfs_ids,
};

//------------------------------------------------------------------------------
// Module init and exit functions
static int __init lb_sysfs_init(void)
{
	int err;
	printk(KERN_INFO "Smart LB to sysfs module starts\n");
	err = platform_driver_register(&lb_sysfs_driver);
	if(err) {
		printk(KERN_INFO "Unable to register Smart LB to sysfs module driver starts\n");
		return err;
	};

	return 0;
};

static void __exit lb_sysfs_exit(void)
{
	printk(KERN_INFO "Smart LB to sysfs module exits\n");
	platform_driver_unregister(&lb_sysfs_driver);
};

module_init(lb_sysfs_init);
module_exit(lb_sysfs_exit);

MODULE_LICENSE("Dual BSD/GPL");
//MODULE_AUTHOR("Marchuk Liliya");
MODULE_DESCRIPTION("Module for exporting Smart Loopback management in sysfs");
