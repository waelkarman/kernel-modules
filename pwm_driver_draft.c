/**
*   This kernel driver is just for testing the PWM.
*
*/

#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/string.h>

#define MIN_NUM_DEV_REQ 1

DEFINE_SEMAPHORE(mtx_0, 1);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wael Karman");
MODULE_DESCRIPTION("A PWM test kernel module");



char str_buf[] = "Hey man test pwm !";


/*DEFINE VERBS FOR FILE OPERATIONS*/
static ssize_t buffer_driver_read(struct file *filp, char __user *buf, size_t count, loff_t *offset){

    return 0;
}

static ssize_t buffer_driver_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset){

    return 0;
}

loff_t buffer_driver_llseek(struct file *filp, loff_t offset, int whence){
    loff_t retval = -EINVAL;
    return retval;
}

static long buffer_driver_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long __arg){
    return 0;
}

static int buffer_driver_open(struct inode *inode, struct file *filp){
    return 0;
}

static int buffer_driver_release(struct inode *inode, struct file *filp){
    return 0;
}



const struct file_operations buffer_driver_fops = {
	.owner	 = THIS_MODULE,
	.llseek	 = buffer_driver_llseek,
	.read	 = buffer_driver_read,
	.write	 = buffer_driver_write,
	.compat_ioctl   = buffer_driver_compat_ioctl,
   	.open	 = buffer_driver_open,
	.release = buffer_driver_release,
};





dev_t dev;  //contains the major number after dynamic allocation
struct cdev* buffer_driver_cdev = NULL; // create a char device instance that kernel knows how to deal with  
struct class *buffer_driver_class;

/*DEFINE LOADING-UNLOADING BEHAVIOUR */
static int __init buffer_driver_init(void)
{
    int res0 = alloc_chrdev_region(&dev,0,MIN_NUM_DEV_REQ, "pwm_driver"); // dynamically allocate major and minor number requested
    
    if(res0 == 0){
        printk(KERN_ALERT "DEVICE DRIVER Major Minor registraion success");    
    }else{
        printk(KERN_ALERT "REGISTRATION Major Minor FAILED");
    }

    buffer_driver_cdev = cdev_alloc();
    cdev_init(buffer_driver_cdev, &buffer_driver_fops); //associate char device with file opertions struct
    int res1 = cdev_add(buffer_driver_cdev ,dev ,MIN_NUM_DEV_REQ); // fill char device struct with the information abot major and minor

    if(res1 == 0){
        printk(KERN_ALERT "DEVICE kernel registration SUCCESS"); 
    }else{
        printk(KERN_ALERT "DEVICE kernel registraion FAILED"); 
    }

    buffer_driver_class = class_create( "buffer_driver"); // crete a class in /sys/class filesystem
    device_create(buffer_driver_class, NULL, buffer_driver_cdev->dev, NULL, "pwm_driver"); // create a device file in /dev/
    
    printk(KERN_ALERT "DEVICE FILE creation .. "); 

    printk(KERN_ALERT "Hello world thats a pwm_driver!   ~   COMM: %s ,PID: %i \n",current->comm, current->pid);
    return 0;    
}

static void __exit buffer_driver_cleanup(void)
{
    device_destroy(buffer_driver_class, buffer_driver_cdev->dev);
    class_destroy(buffer_driver_class);
    unregister_chrdev_region(dev, 1);
    cdev_del(buffer_driver_cdev);
    printk(KERN_ALERT "Cleaning up module.\n");
}

module_init(buffer_driver_init);
module_exit(buffer_driver_cleanup);
