#include <linux/module.h>

#include <linux/init.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/initval.h>

#define print812(...) printk( "<1>812 " ); printk( __VA_ARGS__ ); printk( "\n" );

MODULE_AUTHOR( "Brett Lesnau, Dennis Cornwell" );
MODULE_LICENSE( "Dual BSD/GPL" );

enum { DEVICE_INTEL, DEVICE_INTEL_ICH4, DEVICE_SIS, DEVICE_NFORCE, DEVICE_ALI };

static struct pci_device_id intel_dev_ids[] = {
	{ PCI_VDEVICE(INTEL, 0x2415), DEVICE_INTEL },	/* 82801AA */
	{ PCI_VDEVICE(INTEL, 0x2425), DEVICE_INTEL },	/* 82901AB */
	{ PCI_VDEVICE(INTEL, 0x2445), DEVICE_INTEL },	/* 82801BA */
	{ PCI_VDEVICE(INTEL, 0x2485), DEVICE_INTEL },	/* ICH3 */
	{ PCI_VDEVICE(INTEL, 0x24c5), DEVICE_INTEL_ICH4 }, /* ICH4 */
	{ PCI_VDEVICE(INTEL, 0x24d5), DEVICE_INTEL_ICH4 }, /* ICH5 */
	{ PCI_VDEVICE(INTEL, 0x25a6), DEVICE_INTEL_ICH4 }, /* ESB */
	{ PCI_VDEVICE(INTEL, 0x266e), DEVICE_INTEL_ICH4 }, /* ICH6 */
	{ PCI_VDEVICE(INTEL, 0x27de), DEVICE_INTEL_ICH4 }, /* ICH7 */
	{ PCI_VDEVICE(INTEL, 0x2698), DEVICE_INTEL_ICH4 }, /* ESB2 */
	{ PCI_VDEVICE(INTEL, 0x7195), DEVICE_INTEL },	/* 440MX */
	{ PCI_VDEVICE(SI, 0x7012), DEVICE_SIS },	/* SI7012 */
	{ PCI_VDEVICE(NVIDIA, 0x01b1), DEVICE_NFORCE },	/* NFORCE */
	{ PCI_VDEVICE(NVIDIA, 0x003a), DEVICE_NFORCE },	/* MCP04 */
	{ PCI_VDEVICE(NVIDIA, 0x006a), DEVICE_NFORCE },	/* NFORCE2 */
	{ PCI_VDEVICE(NVIDIA, 0x0059), DEVICE_NFORCE },	/* CK804 */
	{ PCI_VDEVICE(NVIDIA, 0x008a), DEVICE_NFORCE },	/* CK8 */
	{ PCI_VDEVICE(NVIDIA, 0x00da), DEVICE_NFORCE },	/* NFORCE3 */
	{ PCI_VDEVICE(NVIDIA, 0x00ea), DEVICE_NFORCE },	/* CK8S */
	{ PCI_VDEVICE(NVIDIA, 0x026b), DEVICE_NFORCE },	/* MCP51 */
	{ PCI_VDEVICE(AMD, 0x746d), DEVICE_INTEL },	/* AMD8111 */
	{ PCI_VDEVICE(AMD, 0x7445), DEVICE_INTEL },	/* AMD768 */
	{ PCI_VDEVICE(AL, 0x5455), DEVICE_ALI },   /* Ali5455 */
	{ 0, }
};

// Mapping of nr bd bars per device ID
#if 0
static unsigned int bdbars[] = {
    3, /* DEVICE_INTEL */
    6, /* DEVICE_INTEL_ICH4 */
    3, /* DEVICE_SIS */
    6, /* DEVICE_ALI */
    4, /* DEVICE_NFORCE */
};
#endif

static char* device_id_names[] = {
    "DEVICE_INTEL",
    "DEVICE_INTEL_ICH4",
    "DEVICE_SIS",
    "DEVICE_NFORCE",
    "DEVICE_ALI"
};

static int __devinit snd_simple_create( struct pci_dev *pci, const struct pci_device_id *pci_id ) {
    print812( "In device constructor with IRQ %d", pci->irq );
  
    print812( "Using device ID %ld (%s)", pci_id->driver_data, device_id_names[pci_id->driver_data] );   

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
   .id_table = intel_dev_ids,
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
    }

    return 0;
}

static void __exit hello_exit( void ) {
    printk( "<1>812 Unregistering the module" );

    pci_unregister_driver( &driver );
}

module_init( hello_init );
module_exit( hello_exit );
