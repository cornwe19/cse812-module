#include <linux/module.h>

MODULE_LICENSE( "Dual BSD/GPL" );

static int hello_init( void ) {
    printk( "<1> Brett - Initializing the module." );

    return 0;
}

static void hello_exit( void ) {
    printk( "<1> Brett - Unregistering the module" );
}

module_init( hello_init );
module_exit( hello_exit );
