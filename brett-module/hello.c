#include <linux/module.h>

#include <linux/init.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/initval.h>

#include <linux/kernel.h>
#include <net/sock.h>
#include <net/netlink.h>

#define print812(...) printk( "<1>812 " ); printk( __VA_ARGS__ ); printk( "\n" );

MODULE_LICENSE( "Dual BSD/GPL" );

enum { DEVICE_SIMPLE };

static struct pci_device_id snd_simple_ids[] =  {
	{ PCI_VDEVICE(INTEL, 0x2415), DEVICE_SIMPLE },	/* 82801AA */
	{ PCI_VDEVICE(INTEL, 0x2425), DEVICE_SIMPLE },	/* 82901AB */
	{ PCI_VDEVICE(INTEL, 0x2445), DEVICE_SIMPLE },	/* 82801BA */
	{ PCI_VDEVICE(INTEL, 0x2485), DEVICE_SIMPLE },	/* ICH3 */
	{ PCI_VDEVICE(INTEL, 0x24c5), DEVICE_SIMPLE }, /* ICH4 */
	{ PCI_VDEVICE(INTEL, 0x24d5), DEVICE_SIMPLE }, /* ICH5 */
	{ PCI_VDEVICE(INTEL, 0x25a6), DEVICE_SIMPLE }, /* ESB */
	{ PCI_VDEVICE(INTEL, 0x266e), DEVICE_SIMPLE }, /* ICH6 */
	{ PCI_VDEVICE(INTEL, 0x27de), DEVICE_SIMPLE }, /* ICH7 */
	{ PCI_VDEVICE(INTEL, 0x2698), DEVICE_SIMPLE }, /* ESB2 */
	{ PCI_VDEVICE(INTEL, 0x7195), DEVICE_SIMPLE },	/* 440MX */
	{ PCI_VDEVICE(SI, 0x7012), DEVICE_SIMPLE },	/* SI7012 */
	{ PCI_VDEVICE(NVIDIA, 0x01b1), DEVICE_SIMPLE },	/* NFORCE */
	{ PCI_VDEVICE(NVIDIA, 0x003a), DEVICE_SIMPLE },	/* MCP04 */
	{ PCI_VDEVICE(NVIDIA, 0x006a), DEVICE_SIMPLE },	/* NFORCE2 */
	{ PCI_VDEVICE(NVIDIA, 0x0059), DEVICE_SIMPLE },	/* CK804 */
	{ PCI_VDEVICE(NVIDIA, 0x008a), DEVICE_SIMPLE },	/* CK8 */
	{ PCI_VDEVICE(NVIDIA, 0x00da), DEVICE_SIMPLE },	/* NFORCE3 */
	{ PCI_VDEVICE(NVIDIA, 0x00ea), DEVICE_SIMPLE },	/* CK8S */
	{ PCI_VDEVICE(NVIDIA, 0x026b), DEVICE_SIMPLE },	/* MCP51 */
	{ PCI_VDEVICE(AMD, 0x746d), DEVICE_SIMPLE },	/* AMD8111 */
	{ PCI_VDEVICE(AMD, 0x7445), DEVICE_SIMPLE },	/* AMD768 */
	{ PCI_VDEVICE(AL, 0x5455), DEVICE_SIMPLE },   /* Ali5455 */
	{ 0, }
};

static struct sock *my_nl_sock;

DEFINE_MUTEX(my_mutex);

static int
my_rcv_msg(struct sk_buff *skb, struct nlmsghdr *nlh)
{
    int type;
    char *data;

    type = nlh->nlmsg_type;
    if (type != MY_MSG_TYPE) {
        printk("%s: expect %#x got %#x\n", __func__, MY_MSG_TYPE, type);
        return -EINVAL;
    }

    data = NLMSG_DATA(nlh);
    printk("%s: %02x %02x %02x %02x %02x %02x %02x %02x\n", __func__,
            data[0], data[1], data[2], data[3],
            data[4], data[5], data[6], data[7]);
    return 0;
}

static void
my_nl_rcv_msg(struct sk_buff *skb)
{
    mutex_lock(&my_mutex);
    netlink_rcv_skb(skb, &my_rcv_msg);
    mutex_unlock(&my_mutex);
}

static int __devinit snd_simple_create( struct pci_dev *pci, const struct pci_device_id *pci_id ) {
   print812( "In device constructor with IRQ %d", pci->irq );

   return 0;
}

static void __devexit snd_simple_exit( struct pci_dev *pci ) {
   print812( "Stopping the PCI device" );
   
   // free sound card
   // snd_card_free( card );
   pci_set_drvdata( pci, NULL );
}

static struct pci_driver driver = { 
   .name = "Simple Driver",
   .id_table = snd_simple_ids,
   .probe = snd_simple_create,
   // Wrap .remove to turn the exit function into a NULL pointer if
   // the driver is compiled into the kernel
   .remove = __devexit_p( snd_simple_exit ) 
};

static int __init hello_init( void ) {
    print812( "Initializing the module." );

    int result = pci_register_driver( &driver );
    if ( result < 0 ) {
        print812( "Failed to register device - errno: %d", result );
        return result;
    } else {
        print812( "Registered OK with code %d.", result );
        
        my_nl_sock = netlink_kernel_create(&init_net, NETLINK_USERSOCK, 0, my_nl_rcv_msg, NULL, THIS_MODULE);
        if (!my_nl_sock) 
        {
          printk(KERN_ERR "%s: receive handler registration failed\n", __func__);
          return -ENOMEM;
        }
    }

    return 0;
}

static void __exit hello_exit( void ) {
    printk( "<1>812 Unregistering the module" );

    if (my_nl_sock) 
    {
      netlink_kernel_release(my_nl_sock);
    }

    pci_unregister_driver( &driver );
}

module_init( hello_init );
module_exit( hello_exit );
