#include <linux/module.h>

#include <linux/init.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
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

static irqreturn_t snd_simple_interrupt( int irq, void *priv_data ) {
    print812( "Getting an interrupt" );

    return IRQ_HANDLED;
}

struct simplechip {
    struct snd_card *card;
};

static int simplechip_free( struct simplechip *chip ) {
    // TODO: Free the chip

    return 0;
}

static int simplechip_dev_free( struct snd_device *device ) {
    return simplechip_free( (struct simplechip *)device->device_data );
}

static int  __devinit create_chip( struct snd_card *card, struct pci_dev *pci, struct simplechip **rchip ) {
    struct simplechip *chip;
    int                err;
    static struct snd_device_ops ops = {
        .dev_free = simplechip_dev_free,
    };

    *rchip = NULL;

    chip = kzalloc( sizeof(*chip), GFP_KERNEL );
    if ( chip == NULL ) {
        return -ENOMEM;
    }

    chip->card = card;

    err = snd_device_new( card, SNDRV_DEV_LOWLEVEL, chip, &ops );
    if ( err < 0 ) {
        print812( "Failed to set card device (%d)", err );

        simplechip_free( chip );

        return err;
    }

    snd_card_set_dev( card, &pci->dev );

    *rchip = chip;
    return 0;
}

static int __devinit snd_simple_probe( struct pci_dev *pci, const struct pci_device_id *pci_id ) {
    int err = 0;
    struct snd_card *card;
    struct simplechip *chip;

    print812( "In device constructor with IRQ %d", pci->irq );
    print812( "Using device ID %ld (%s)", pci_id->driver_data, device_id_names[pci_id->driver_data] );   

    err = snd_card_create( SNDRV_DEFAULT_IDX1, SNDRV_DEFAULT_STR1, THIS_MODULE, 0, &card );
    if ( err < 0 ) {
        print812( "Failed to create soundcard (%d)", err );
        return err;
    }
  
    err = create_chip( card, pci, &chip );
    if ( err < 0 ) {
        print812( "Failed to create chip (%d)", err );

        return err;
    }

    // TODO: fix the null ptr here. Need some data to fill up
    err = request_irq( pci->irq, snd_simple_interrupt, IRQF_SHARED, "Simple Card", NULL );
    if ( err < 0 ) {
        print812( "Failed to get IRQ channel (%d)", err );

        snd_card_free( card );

        return err;
    }

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
   .probe = snd_simple_probe,
   // Wrap .remove to turn the exit function into a NULL pointer if
   // the driver is compiled into the kernel
   .remove = __devexit_p( snd_simple_exit ) 
};

static int __init hello_init( void ) {
    int err = 0;
    
    print812( "Initializing the module." );

    err = pci_register_driver( &driver );
    if ( err < 0 ) {
        print812( "Failed to register device - errno: %d", err );
        return err;
    } else {
        print812( "Registered OK with code %d.", err );
    }

    return 0;
}

static void __exit hello_exit( void ) {
    printk( "<1>812 Unregistering the module" );

    pci_unregister_driver( &driver );
}

module_init( hello_init );
module_exit( hello_exit );
