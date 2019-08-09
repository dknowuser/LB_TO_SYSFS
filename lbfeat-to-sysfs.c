#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/regmap.h>
#include <linux/device.h>

#include <grif_fpga.h>

#define MAJOR_DEV_NUMBER 		14
#define FPGA_FEATURE_LB_BASE_CR 	85
#define PARAMETER_LEN 			2

#define LB_BASE_CR_LB_EN_BIT		0
#define LB_BASE_CR_MAC_SWAP_EN_BIT	LB_BASE_CR_LB_EN_BIT + 1
#define LB_BASE_CR_IP_SWAP_EN_BIT	LB_BASE_CR_MAC_SWAP_EN_BIT + 1
#define LB_BASE_CR_TCP_UDP_SWAP_EN_BIT	LB_BASE_CR_IP_SWAP_EN_BIT + 1
#define LB_BASE_CR_PASSTHROUGH_EN_BIT	LB_BASE_CR_TCP_UDP_SWAP_EN_BIT + 1
#define LB_BASE_CR_PASSTHROUGH_MODE_BIT	LB_BASE_CR_PASSTHROUGH_EN_BIT + 1
#define LB_BASE_CR_LB_L1_EN_BIT		LB_BASE_CR_PASSTHROUGH_MODE_BIT + 1

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

// Function for getting bit shift by sysfs attribute name
unsigned get_shift_by_attrib_name(const char *attrib_name);

//-----------------------------------------------------------------------------
// Global variables section

// Module device table
static const struct of_device_id lb_base_sysfs_ids[] = {
	{ .compatible = "stcmtk,lbfeat-to-sysfs-0", },
	{ },
};

// Uncomment it to enable autoloading
// MODULE_DEVICE_TABLE(of, lb_sysfs_ids);

static const char *LB_BASE_CLASS_NAME			= "smart_lb";
static const char *LB_BASE_DEVICE_NAME			= "lb_";
static const char *LB_BASE_CR_LB_EN_NAME 		= "LB_BASE_CR_LB_EN";
static const char *LB_BASE_CR_MAC_SWAP_EN_NAME 		= "LB_BASE_CR_MAC_SWAP_EN";
static const char *LB_BASE_CR_IP_SWAP_EN_NAME 		= "LB_BASE_CR_IP_SWAP_EN";
static const char *LB_BASE_CR_TCP_UDP_SWAP_EN_NAME 	= "LB_BASE_CR_TCP_UDP_SWAP_EN";
static const char *LB_BASE_CR_PASSTHROUGH_EN_NAME 	= "LB_BASE_CR_PASSTHROUGH_EN";
static const char *LB_BASE_CR_PASSTHROUGH_MODE_NAME 	= "LB_BASE_CR_PASSTHROUGH_MODE";
static const char *LB_BASE_CR_LB_L1_EN_NAME 		= "LB_BASE_CR_LB_L1_EN";

static struct device *lb_base_sysfs_device;
static struct regmap *regmap = NULL;
static struct fpga_feature *lb_base_feat;

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
ssize_t show_lb_base(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	int err;
	unsigned data = 0;
	uint32_t port_number;
	uint16_t cr_offset;
	unsigned shift = get_shift_by_attrib_name(attr->attr.name);

	/*err = device_property_read_u32(lb_base_sysfs_device, "PORT", &port_number);
	if(err) {
		dev_err(dev, "Error reading LB_BASE feature port property from Device Tree\n");
		return snprintf(buf, PAGE_SIZE, "Error reading LB_BASE feature port property from Device Tree\n");
	};*/

	//of_property_read_u32(dev->of_node, "port", &port_number);
	
	//const char *label = of_get_property(dev->of_node, "label", NULL);
	//if(label)
	if(dev->of_node)
		return snprintf(buf, PAGE_SIZE, dev->init_name);


	cr_offset = grif_fpga_feature_cr_base_on_port(lb_base_feat, port_number);
	dev_info(dev, "port_number = %d cr_offset = %d\n", port_number, cr_offset);

	err = regmap_read(regmap, cr_offset, &data);
	if(err) {
		dev_err(dev, "Error reading LB_BASE feature control register\n");
		return snprintf(buf, PAGE_SIZE, "Error reading LB_BASE feature control register\n");
	};

	return snprintf(buf, PAGE_SIZE, (data & ((unsigned)1 << shift))?"1\n":"0\n");
};

ssize_t store_lb_base(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	int err;
	unsigned data = 0;
	unsigned shift = get_shift_by_attrib_name(attr->attr.name);

	if(size != PARAMETER_LEN) {
		dev_info(dev, "Invalid input parameter length (should be equal 1)\n");
		return EINVAL;
	};

	if((*buf != '0') && (*buf != '1')) {
		dev_info(dev, "Invalid input parameter value (should be equal '0' or '1')\n");
		return EINVAL;
	};

	err = regmap_read(regmap, FPGA_FEATURE_LB_BASE_CR, &data);
	if(err) {
		dev_err(dev, "Error reading LB_BASE feature control register\n");
		return EIO;
	};

	if(*buf == '1') {
		data |= ((unsigned)1 << shift);
	}
	else {
		data &= ~((unsigned)1 << shift);
	};

	err = regmap_write(regmap, FPGA_FEATURE_LB_BASE_CR, data);
	if(err) {
		dev_err(dev, "Error writing LB_BASE feature control register\n");
		return EIO;
	};

	return size;
};

static DEVICE_ATTR(LB_BASE_CR_LB_EN, S_IRUGO|S_IWUSR, show_lb_base, store_lb_base);

//-----------------------------------------------------------------------------

// Module probe and remove callbacks
// Function for plugging Smart Loopback FPGA feature into sysfs
static int lb_base_sysfs_probe(struct platform_device *pdev)
{
	struct grif_fpga *g;
	//struct fpga_feature *lb_base_feat;
	uint32_t port_number;
	//char dev_name[5];

	dev_info(&pdev->dev, "Smart LB to sysfs module probing procedure starts\n");
	g = get_grif_fpga(pdev->dev.of_node);
	if(!g) {
		dev_info(&pdev->dev, "FPGA is not presented\n");
		return -ENODEV;
	};

	regmap = g->regmap;
	if(!regmap) {
		dev_warn(&pdev->dev, "Couldn't access FPGA regmap\n");
		return -EPROBE_DEFER;
	};

	lb_base_feat = grif_fpga_get_feature(g, FPGA_FEATURE_LB_BASE);
	if(!lb_base_feat) {
		dev_err(&pdev->dev, "Couldn't get LB_BASE feature\n");
		return -ENODEV;
	};

	if(!of_find_property(pdev->dev.of_node, "port", NULL)) {
		dev_err(&pdev->dev, "Port property is not presented in Device Tree\n");
		return -ENODEV;
	};

	of_property_read_u32(pdev->dev.of_node, "port", &port_number);
	dev_info(&pdev->dev, "port_number = %d\n", port_number);
	if(!((lb_base_feat->port_mask >> port_number) & 1)) {
		dev_err(&pdev->dev, "Port %d is not set in LB_BASE feature port mask\n", port_number);
		return -ENODEV;
	};

	cl = class_create(THIS_MODULE, LB_BASE_CLASS_NAME);
	lb_base_sysfs_device = device_create(cl, NULL, MKDEV(MAJOR_DEV_NUMBER, 0), NULL, LB_BASE_DEVICE_NAME);
	if(!cl) {
		dev_info(&pdev->dev, "Failed to create \"Smart_LB\" class in /sys/class\n");
		return -EPERM;
	};

	add_attribute(lb_base_sysfs_device, LB_BASE_CR_LB_EN_NAME, &dev_attrib_lb_en,
			show_lb_base, store_lb_base);
	add_attribute(lb_base_sysfs_device, LB_BASE_CR_MAC_SWAP_EN_NAME,
			&dev_attrib_mac_swap_en, show_lb_base, store_lb_base);
	add_attribute(lb_base_sysfs_device, LB_BASE_CR_IP_SWAP_EN_NAME,
			&dev_attrib_ip_swap_en, show_lb_base, store_lb_base);
	add_attribute(lb_base_sysfs_device, LB_BASE_CR_TCP_UDP_SWAP_EN_NAME,
			&dev_attrib_tcp_udp_swap_en, show_lb_base,
			store_lb_base);
	add_attribute(lb_base_sysfs_device, LB_BASE_CR_PASSTHROUGH_EN_NAME,
			&dev_attrib_passthrough_en, show_lb_base,
			store_lb_base);
	add_attribute(lb_base_sysfs_device, LB_BASE_CR_PASSTHROUGH_MODE_NAME,
			&dev_attrib_passthrough_mode, show_lb_base,
			store_lb_base);
	add_attribute(lb_base_sysfs_device, LB_BASE_CR_LB_L1_EN_NAME,
			&dev_attrib_lb_l1_en, show_lb_base, store_lb_base);

	return 0;
};

// Function for unplugging Smart Loopback FPGA feature from sysfs
static int lb_base_sysfs_remove(struct platform_device *pdev)
{
	device_remove_file(lb_base_sysfs_device, &dev_attrib_lb_l1_en);
	device_remove_file(lb_base_sysfs_device, &dev_attrib_passthrough_mode);
	device_remove_file(lb_base_sysfs_device, &dev_attrib_passthrough_en);
	device_remove_file(lb_base_sysfs_device, &dev_attrib_tcp_udp_swap_en);
	device_remove_file(lb_base_sysfs_device, &dev_attrib_ip_swap_en);
	device_remove_file(lb_base_sysfs_device, &dev_attrib_mac_swap_en);
	device_remove_file(lb_base_sysfs_device, &dev_attrib_lb_en);

	device_destroy(cl, MKDEV(MAJOR_DEV_NUMBER, 0));
	class_destroy(cl);

	dev_info(&pdev->dev, "Smart LB to sysfs module has been removed\n");
	return 0;
};

// Module driver structure
static struct platform_driver lb_base_sysfs_driver = {
	.driver = {
		.name = "lbfeat-to-sysfs",
		.of_match_table = of_match_ptr(lb_base_sysfs_ids),
	},
	.probe = lb_base_sysfs_probe,
	.remove = lb_base_sysfs_remove,
};

//------------------------------------------------------------------------------
// Module init and exit functions
static int __init lb_base_sysfs_init(void)
{
	int err;
	printk(KERN_INFO "Smart LB to sysfs module starts\n");
	err = platform_driver_register(&lb_base_sysfs_driver);
	if(err) {
		printk(KERN_INFO "Unable to register Smart LB to sysfs module driver starts\n");
		return err;
	};

	return 0;
};

static void __exit lb_base_sysfs_exit(void)
{
	printk(KERN_INFO "Smart LB to sysfs module exits\n");
	platform_driver_unregister(&lb_base_sysfs_driver);
};

module_init(lb_base_sysfs_init);
module_exit(lb_base_sysfs_exit);

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

unsigned get_shift_by_attrib_name(const char *attrib_name)
{
	if(!strcmp(attrib_name, LB_BASE_CR_LB_EN_NAME))
		return LB_BASE_CR_LB_EN_BIT;
	else
		if(!strcmp(attrib_name, LB_BASE_CR_MAC_SWAP_EN_NAME))
			return LB_BASE_CR_MAC_SWAP_EN_BIT;
		else
			if(!strcmp(attrib_name, LB_BASE_CR_IP_SWAP_EN_NAME))
				return LB_BASE_CR_IP_SWAP_EN_BIT;
			else
				if(!strcmp(attrib_name, LB_BASE_CR_TCP_UDP_SWAP_EN_NAME))
					return LB_BASE_CR_TCP_UDP_SWAP_EN_BIT;
				else
					if(!strcmp(attrib_name, LB_BASE_CR_PASSTHROUGH_EN_NAME))
						return LB_BASE_CR_PASSTHROUGH_EN_BIT;
					else
						if(!strcmp(attrib_name, LB_BASE_CR_PASSTHROUGH_MODE_NAME))
							return LB_BASE_CR_PASSTHROUGH_MODE_BIT;
						else
							return LB_BASE_CR_LB_L1_EN_BIT;
};
