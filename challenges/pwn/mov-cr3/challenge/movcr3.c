#include <linux/atomic.h> 
#include <linux/cdev.h> 
#include <linux/delay.h> 
#include <linux/device.h> 
#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/kernel.h> /* for sprintf() */ 
#include <linux/module.h> 
#include <linux/printk.h> 
#include <linux/types.h> 
#include <linux/uaccess.h> /* for get_user and put_user */ 
#include <linux/version.h> 
#include <asm/errno.h> 
 
/*  Prototypes - this would normally go in a .h file */ 
static long device_ioctl(struct file *filp, unsigned int cmd, 
                             unsigned long arg);
 
#define SUCCESS 0 
#define DEVICE_NAME "movcr3" /* Dev name as it appears in /proc/devices   */ 
#define IOCTL_READ          0x1001
#define IOCTL_READ_WITH_CR3 0x1002

/* Global variables are declared as static, so are global within the file. */ 
 
static int major; /* major number assigned to our device driver */ 
 
enum { 
    CDEV_NOT_USED = 0, 
    CDEV_EXCLUSIVE_OPEN = 1, 
}; 

struct ioctl_read_kernel_arg {
    size_t dst;
    size_t src;
    size_t n;
};

struct ioctl_read_user_arg {
    void *dst;
    void *src;
    size_t cr3_from;
    size_t cr3_to;
};

static struct class *cls;  
static struct file_operations chardev_fops = { 
    .unlocked_ioctl = device_ioctl, 
}; 

static int __init chardev_init(void) 
{ 
    major = register_chrdev(0, DEVICE_NAME, &chardev_fops); 
 
    if (major < 0) { 
        pr_alert("Registering char device failed with %d\n", major); 
        return major; 
    } 
 
    pr_info("I was assigned major number %d.\n", major); 
 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0) 
    cls = class_create(DEVICE_NAME); 
#else 
    cls = class_create(THIS_MODULE, DEVICE_NAME); 
#endif 
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME); 
 
    pr_info("Device created on /dev/%s\n", DEVICE_NAME); 
 
    return SUCCESS; 
} 
 
static void __exit chardev_exit(void) 
{ 
    device_destroy(cls, MKDEV(major, 0)); 
    class_destroy(cls); 
 
    /* Unregister the device */ 
    unregister_chrdev(major, DEVICE_NAME); 
} 
 
/* Methods */ 
static long device_ioctl(struct file *filp, unsigned int cmd, 
                             unsigned long arg) 
{
    long retval = 0;
    long val = 0;
    struct ioctl_read_user_arg data_user;
    struct ioctl_read_kernel_arg data_kernel;

    // initialize struct var
    memset(&data_user, 0, sizeof(data_user));
    memset(&data_kernel, 0, sizeof(data_kernel));

    switch (cmd) {
        // AAR in kernel-land
        case IOCTL_READ:
            if (copy_from_user(&data_kernel, (int __user *)arg, sizeof(data_kernel))) {
                retval = -EFAULT;
                goto done;
            }

            if (copy_to_user((void *)data_kernel.dst, (void *)data_kernel.src, data_kernel.n)) {
                retval = -EFAULT;
                goto done;
            }
            break;
        
        // AAR in user-land with CR3 register
        case IOCTL_READ_WITH_CR3:
            if (copy_from_user(&data_user, (int __user *)arg, sizeof(data_user))) {
                retval = -EFAULT;
                goto done;
            }

            asm volatile ("mov %0, %%cr3" : : "r" (data_user.cr3_to));
            if (copy_from_user(&val, (int __user *)data_user.src, 8)) {
                retval = -EFAULT;
                goto done;
            }
            
            asm volatile ("mov %0, %%cr3" : : "r" (data_user.cr3_from));
            if (copy_to_user((int __user *)data_user.dst, &val, 8)) {
                retval = -EFAULT;
                goto done;
            }
            
            break;
    }

done:
    return retval;
}

module_init(chardev_init); 
module_exit(chardev_exit); 
 
MODULE_LICENSE("GPL");
