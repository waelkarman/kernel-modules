/**
*   This kernel driver is just a buffer where to read and write 
*   for practice memory allocation in kernel space.
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
MODULE_DESCRIPTION("A Simple buffer as a kernel module");


static size_t current_size = 0;
static size_t current_index = 0;
char str_buf[] = "Hey man the buffer is empty!";
bool no_write = true;

/*DEFINE VERBS FOR FILE OPERATIONS*/
static ssize_t buffer_driver_read(struct file *filp, char __user *buf, size_t count, loff_t *offset){
    down_interruptible(&mtx_0); // semaphore lock
    
    ssize_t read = 0;
    if (*offset == 0 && no_write){
        copy_to_user(buf, filp->private_data + *offset ,strlen(str_buf));
        *offset += strlen(str_buf);
        read = strlen(str_buf);
    }else{
        if(strlen(filp->private_data) > *offset){
            copy_to_user(buf, filp->private_data + *offset, current_size - *offset);
            read = current_size - *offset;
            *offset += current_size - *offset;
        }
    }

    up(&mtx_0); // semaphore unlock
    
    return read;
}

static ssize_t buffer_driver_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset){
    down_interruptible(&mtx_0); // semaphore lock
    
    if (*offset == 0){
        kfree(filp->private_data);
        filp->private_data=NULL;
        current_size=0;
        no_write=false;
    }

    if (filp->private_data == NULL){
        filp->private_data = kmalloc((count)*sizeof(char),GFP_KERNEL);
    }else{
        for (ssize_t i = 0; i < count; i++) {
            if (((char *)filp->private_data)[i] == '\0') {
                ((char *)filp->private_data)[i] = ' ';
            }
        }

        filp->private_data = krealloc(filp->private_data,current_size+(count)*sizeof(char),GFP_KERNEL);
    }
    current_size+=count;
    copy_from_user(filp->private_data + *offset,buf,count);
    ((char *)filp->private_data)[current_size-1] = '\0';
    printk(KERN_ALERT "KERNEL SPACE - received: %s of character %zu ",(char *)filp->private_data + *offset, count);

    up(&mtx_0); // semaphore unlock
    
    return count;
}

loff_t buffer_driver_llseek(struct file *filp, loff_t offset, int whence){
    loff_t retval = -EINVAL;
    return retval;
}

static long buffer_driver_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long __arg){
    printk(KERN_ALERT "ioctl command");
    return 0;
}

static int buffer_driver_open(struct inode *inode, struct file *filp){
    printk(KERN_ALERT "Load Driver");
    filp->private_data = kmalloc((strlen(str_buf))*sizeof(char), GFP_KERNEL);
    strncpy(filp->private_data, str_buf, strlen(str_buf));
    ((char *)filp->private_data)[strlen(str_buf)] = '\0';
    current_size+=strlen(str_buf);
    return 0;
}

static int buffer_driver_release(struct inode *inode, struct file *filp){
    printk(KERN_ALERT "Remove Driver");
    kfree(filp->private_data);
    filp->private_data=NULL;
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
    int res0 = alloc_chrdev_region(&dev,0,MIN_NUM_DEV_REQ, "buffer_driver"); // dynamically allocate major and minor number requested
    
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
    device_create(buffer_driver_class, NULL, buffer_driver_cdev->dev, NULL, "buffer_driver"); // create a device file in /dev/
    
    printk(KERN_ALERT "DEVICE FILE creation .. "); 

    printk(KERN_ALERT "Hello world thats a buffer_driver!   ~   COMM: %s ,PID: %i \n",current->comm, current->pid);
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
