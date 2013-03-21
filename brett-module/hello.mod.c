#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x8479aeaa, "module_layout" },
	{ 0x98526078, "netlink_kernel_create" },
	{ 0x40918e5e, "init_net" },
	{ 0x699e337c, "__pci_register_driver" },
	{ 0x3a322124, "dev_set_drvdata" },
	{ 0xc1d9645b, "pci_unregister_driver" },
	{ 0x24b73462, "sock_release" },
	{ 0xea147363, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

