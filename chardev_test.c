#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#include <linux/string.h>

#define MIN_NUM_DEV_REQ 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wael Karman");
MODULE_DESCRIPTION("A Simple Hello World module");

static int param1 = 1;
static char* param2 = "Pass a second argument.";
module_param(param1, int, S_IRUGO); //last input is a permission parameter defined in linux/stat.h
module_param(param2, charp, S_IRUGO); //can be used even the function:  module_param_array();

char* str = "HEY MAN YOU ARE CORRECTLY READING.";
int character_to_read = 34;
int step = 1;
int index = 0;

/*DEFINE VERBS FOR FILE OPERATIONS*/
loff_t chardev_test_llseek(struct file *filp, loff_t offset, int whence){
    loff_t retval = -EINVAL;
    return retval;
}

static ssize_t chardev_test_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos){
    if((character_to_read - index) < 0){
        ssize_t ret = 0;
        return ret;
    }
    copy_to_user(buf, str+index ,step);
    index = index + step;
    ssize_t ret = step;
    return ret;
}


static ssize_t chardev_test_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos){
    printk(KERN_ALERT "BUFFER SIZE:  %d\n ",count);
    filp->private_data = kmalloc(count*sizeof(char*), GFP_KERNEL);
    copy_from_user(filp->private_data,buf,count);

    int writed_data = strlen((char*)filp->private_data);

    printk(KERN_ALERT "RECEIVED STRING: %s of character %d\n ",(char*)filp->private_data,writed_data);
    kfree(filp->private_data);
    buf = NULL;
    filp->private_data = NULL;
    ssize_t ret = writed_data;
    *ppos += count;
    return ret;
}

static long chardev_test_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long __arg){
    return 0;
}

static int chardev_test_open(struct inode *inode, struct file *filp){
    printk(KERN_ALERT "open device");
    return 0;
}

static int chardev_test_release(struct inode *inode, struct file *filp){
    printk(KERN_ALERT "close device");
    return 0;
}

const struct file_operations chardev_test_fops = {
	.owner	 = THIS_MODULE,
	.llseek	 = chardev_test_llseek,
	.read	 = chardev_test_read,
	.write	 = chardev_test_write,
	.compat_ioctl   = chardev_test_compat_ioctl,
    .open	 = chardev_test_open,
	.release = chardev_test_release,
};





dev_t dev;  //contains the major number after dynamic allocation
struct cdev* chardev_test_cdev = NULL; // create a char device instance that kernel knows how to deal with  
struct class *chardev_test_class;

/*DEFINE LOADING-UNLOADING BEHAVIOUR */
static int __init chardev_test_init(void)
{
    int res0 = alloc_chrdev_region(&dev,0,MIN_NUM_DEV_REQ, "chardev_test"); // dynamically allocate major and minor number requested
    
    if(res0 == 0){
        printk(KERN_ALERT "DEVICE DRIVER Major Minor registraion success");    
    }else{
        printk(KERN_ALERT "REGISTRATION Major Minor FAILED");
    }

    chardev_test_cdev = cdev_alloc();
    cdev_init(chardev_test_cdev, &chardev_test_fops); //associate char device with file opertions struct
    int res1 = cdev_add(chardev_test_cdev ,dev ,MIN_NUM_DEV_REQ); // fill char device struct with the information abot major and minor

    if(res1 == 0){
        printk(KERN_ALERT "DEVICE kernel registration SUCCESS"); 
    }else{
        printk(KERN_ALERT "DEVICE kernel registraion FAILED"); 
    }

    chardev_test_class = class_create(THIS_MODULE, "chardev_test"); // crete a class in /sys/class filesystem
    device_create(chardev_test_class, NULL, dev, NULL, "chardev_test1"); // create a device file in /dev/
    
    printk(KERN_ALERT "DEVICE FILE creation .. "); 

    printk(KERN_ALERT "Hello world thats a chardev_test! %x %x , COMM: %s ,PID: %i \n",param1,param2,current->comm, current->pid);
    return 0;    
}

static void __exit chardev_test_cleanup(void)
{
    unregister_chrdev_region(dev, 1);
    cdev_del(chardev_test_cdev);
    printk(KERN_ALERT "Cleaning up module.\n");
}

module_init(chardev_test_init);
module_exit(chardev_test_cleanup);



/*EXPORT A SYMBOT TO USE IT FROM APPLICATIONS AND OTHER MODULES*/
int exp_func(int i)
{
    pr_info("%s:%d the value passed in is %d\n",
            __func__, __LINE__, i);

    return i;
}
EXPORT_SYMBOL(exp_func);


