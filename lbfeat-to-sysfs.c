#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/regmap.h>
#include <linux/device.h>

#include <grif_fpga.h>

#define MAJOR_DEV_NUMBER 14

//-----------------------------------------------------------------------------
// Forward declarations

// Function for adding sysfs attributes
void add_attribute(struct device *dev, const char *attrib_name,
		struct device_attribute *dev_attrib,
		ssize_t (*show)(struct device *dev,
		struct device_attribute *attr, char *buf),
		ssize_t (*store)(struct device *dev,
		struct device_attribute *attr, const char *buf,
		size_t count));

//-----------------------------------------------------------------------------
// Global variables section

// Module device table
static const struct of_device_id lb_sysfs_ids[] = {
	{ .compatible = "stcmtk,lbfeat-to-sysfs", },
	{ },
};

// Uncomment it to enable autoloading
// MODULE_DEVICE_TABLE(of, lb_sysfs_ids);

static struct device *lb_sysfs_device;
static struct regmap *regmap = NULL;
static struct class *cl;
static struct attribute attrib;
static struct device_attribute dev_attrib_lb_en, dev_attrib_mac_swap_en,
			       dev_attrib_ip_swap_en, dev_attrib_tcp_udp_swap_en,
			       dev_attrib_passthrough_en, dev_attrib_passthrough_mode,
			       dev_attrib_lb_l1_en;

//-----------------------------------------------------------------------------
// Callback section

// Callbacks for reading and writing sysfs attributes
// Callbacks for Smart LB Enable bit
ssize_t show_lb_en(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	return 0;
};

ssize_t store_lb_en(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	return 0;
};

// Callbacks for Smart LB MAC Swap Enable bit
ssize_t show_mac_swap_en(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	return 0;
};

ssize_t store_mac_swap_en(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	return 0;
};

// Callbacks for Smart LB IP Swap Enable bit
ssize_t show_ip_swap_en(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	return 0;
};

ssize_t store_ip_swap_en(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	return 0;
};

// Callbacks for Smart LB TCP UDP Swap Enable bit
ssize_t show_tcp_udp_swap_en(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	return 0;
};

ssize_t store_tcp_udp_swap_en(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	return 0;
};

// Callbacks for Smart LB Passthrough Enable bit
ssize_t show_passthrough_en(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	return 0;
};

ssize_t store_passthrough_en(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	return 0;
};

// Callbacks for Smart LB Passthrough Mode bit
ssize_t show_passthrough_mode(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	return 0;
};

ssize_t store_passthrough_mode(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	return 0;
};

// Callbacks for Smart LB L1 Enable bit
ssize_t show_lb_l1_en(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	return 0;
};

ssize_t store_lb_l1_en(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	return 0;
};

static DEVICE_ATTR(LB_BASE_CR_LB_EN, S_IRUGO|S_IWUSR, show_lb_en, store_lb_en);
static DEVICE_ATTR(LB_BASE_CR_MAC_SWAP_EN, S_IRUGO|S_IWUSR, show_mac_swap_en,
		store_mac_swap_en);
static DEVICE_ATTR(LB_BASE_CR_IP_SWAP_EN, S_IRUGO|S_IWUSR, show_ip_swap_en,
		store_ip_swap_en);
static DEVICE_ATTR(LB_BASE_CR_TCP_UDP_SWAP_EN, S_IRUGO|S_IWUSR,
		show_tcp_udp_swap_en, store_tcp_udp_swap_en);
static DEVICE_ATTR(LB_BASE_CR_PASSTHROUGH_EN, S_IRUGO|S_IWUSR,
		show_passthrough_en, store_passthrough_en);
static DEVICE_ATTR(LB_BASE_CR_PASSTHROUGH_MODE, S_IRUGO|S_IWUSR,
		show_passthrough_mode, store_passthrough_mode);
static DEVICE_ATTR(LB_BASE_CR_LB_L1_EN, S_IRUGO|S_IWUSR, show_lb_l1_en,
		store_lb_l1_en);

//-----------------------------------------------------------------------------

// Module probe and remove callbacks
// Function for plugging Smart Loopback FPGA feature into sysfs
static int lb_sysfs_probe(struct platform_device *pdev)
{
	struct grif_fpga *g;

	dev_info(&pdev->dev, "Smart LB to sysfs module probing procedure starts\n");
	g = get_grif_fpga(pdev->dev.of_node);
	if(!g) {
		dev_info(&pdev->dev, "FPGA is not presented\n");
		return -ENODEV;
	};

	regmap = g->regmap;

	cl = class_create(THIS_MODULE, "smart_lb");
	lb_sysfs_device = device_create(cl, NULL, MKDEV(MAJOR_DEV_NUMBER, 0), NULL, "lb" );
	if(!cl) {
		dev_info(&pdev->dev, "Failed to create \"Smart_LB\" class in /sys/class\n");
		return -EPERM;
	};

	add_attribute(lb_sysfs_device, "LB_BASE_CR_LB_EN", &dev_attrib_lb_en,
			show_lb_en, store_lb_en);
	add_attribute(lb_sysfs_device, "LB_BASE_CR_MAC_SWAP_EN",
			&dev_attrib_mac_swap_en, show_mac_swap_en, store_mac_swap_en);
	add_attribute(lb_sysfs_device, "LB_BASE_CR_IP_SWAP_EN",
			&dev_attrib_ip_swap_en, show_ip_swap_en, store_ip_swap_en);
	add_attribute(lb_sysfs_device, "LB_BASE_CR_TCP_UDP_SWAP_EN",
			&dev_attrib_tcp_udp_swap_en, show_tcp_udp_swap_en,
			store_tcp_udp_swap_en);
	add_attribute(lb_sysfs_device, "LB_BASE_CR_PASSTHROUGH_EN",
			&dev_attrib_passthrough_en, show_passthrough_en,
			store_passthrough_en);
	add_attribute(lb_sysfs_device, "LB_BASE_CR_PASSTHROUGH_MODE",
			&dev_attrib_passthrough_mode, show_passthrough_mode,
			store_passthrough_mode);
	add_attribute(lb_sysfs_device, "LB_BASE_CR_LB_L1_EN",
			&dev_attrib_lb_l1_en, show_lb_l1_en, store_lb_l1_en);

	return 0;
};

// Function for unplugging Smart Loopback FPGA feature from sysfs
static int lb_sysfs_remove(struct platform_device *pdev)
{
	device_remove_file(lb_sysfs_device, &dev_attrib_lb_l1_en);
	device_remove_file(lb_sysfs_device, &dev_attrib_passthrough_mode);
	device_remove_file(lb_sysfs_device, &dev_attrib_passthrough_en);
	device_remove_file(lb_sysfs_device, &dev_attrib_tcp_udp_swap_en);
	device_remove_file(lb_sysfs_device, &dev_attrib_ip_swap_en);
	device_remove_file(lb_sysfs_device, &dev_attrib_mac_swap_en);
	device_remove_file(lb_sysfs_device, &dev_attrib_lb_en);

	device_destroy(cl, MKDEV(MAJOR_DEV_NUMBER, 0));
	class_destroy(cl);

	dev_info(&pdev->dev, "Smart LB to sysfs module has been removed\n");
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

//-----------------------------------------------------------------------------
// Miscellaneous functions

// Function for adding sysfs attributes
void add_attribute(struct device *dev, const char *attrib_name,
		struct device_attribute *dev_attrib,
		ssize_t (*show)(struct device *dev,
		struct device_attribute *attr, char *buf),
		ssize_t (*store)(struct device *dev,
		struct device_attribute *attr, const char *buf,
		size_t count))
{
	int err;
	attrib.mode = S_IRUGO | S_IWUGO;
	attrib.name = attrib_name;
	dev_attrib->attr = attrib;
	dev_attrib->show = show;
	dev_attrib->store = store;
	err = device_create_file(dev, dev_attrib);
	if(err < 0)
		dev_info(dev, "Error creating file %s\n", attrib.name);
};

