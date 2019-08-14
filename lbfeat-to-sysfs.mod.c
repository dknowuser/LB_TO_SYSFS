#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa811f2ad, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x400d84b6, __VMLINUX_SYMBOL_STR(regmap_write) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x533a2a6b, __VMLINUX_SYMBOL_STR(regmap_read) },
	{ 0xb026df16, __VMLINUX_SYMBOL_STR(grif_fpga_feature_cr_base_on_port) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0x9edf229e, __VMLINUX_SYMBOL_STR(dev_warn) },
	{ 0xffb552ec, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0xedc94196, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x209834ce, __VMLINUX_SYMBOL_STR(grif_fpga_get_feature) },
	{ 0xbdf44631, __VMLINUX_SYMBOL_STR(get_grif_fpga) },
	{ 0xf20fde86, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xfb3e4395, __VMLINUX_SYMBOL_STR(devm_kmalloc) },
	{ 0xf176fbca, __VMLINUX_SYMBOL_STR(of_property_read_variable_u32_array) },
	{ 0xbd879420, __VMLINUX_SYMBOL_STR(of_find_property) },
	{ 0xe99ed379, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0x5339f3b6, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x9eefbf5c, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0xc3483ba4, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xee066fa0, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x92b6d872, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x307ff672, __VMLINUX_SYMBOL_STR(device_remove_file) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=grif_fpga_mgr";


MODULE_INFO(srcversion, "5D24B26DB52DE5B3E1E0414");
