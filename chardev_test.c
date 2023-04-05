#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define MIN_NUM_DEV_REQ 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wael Karman");
MODULE_DESCRIPTION("A Simple Hello World module");

static int param1 = 1;
static char* param2 = "Pass a second argument.";
module_param(param1, int, S_IRUGO); //last input is a permission parameter defined in linux/stat.h
module_param(param2, charp, S_IRUGO); //can be used even the function:  module_param_array();

int character_to_read = 34;
int i = 0;

/*DEFINE VERBS FOR FILE OPERATIONS*/
loff_t chardev_test_llseek(struct file *file, loff_t offset, int whence){
    loff_t retval = -EINVAL;
    return retval;
}

static ssize_t chardev_test_read(struct file *file, char __user *buf, size_t count, loff_t *ppos){
    char* str = "HEY MAN YOU ARE CORRECTLY READING.";
    copy_to_user(buf, str+i ,1);
    character_to_read = character_to_read-1;
    i++;
    ssize_t ret = character_to_read;
    return ret;
}

static ssize_t chardev_test_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos){
    ssize_t ret = 0;
    return ret;
}

static long chardev_test_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long __arg){
    return 0;
}

static int chardev_test_open(struct inode *inode, struct file *flip){
    printk(KERN_ALERT "open device");
    return 0;
}

static int chardev_test_release(struct inode *inode, struct file *flip){
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





dev_t dev;
struct cdev* chardev_test_cdev = NULL;
struct class *chardev_test_class;

/*DEFINE LOADING-UNLOADING BEHAVIOUR */
static int __init chardev_test_init(void)
{
    int res0 = alloc_chrdev_region(&dev,0,MIN_NUM_DEV_REQ, "chardev_test");
    
    if(res0 == 0){
        printk(KERN_ALERT "DEVICE DRIVER registraion success");    
    }else{
        printk(KERN_ALERT "REGISTRATION FAILED");
    }


    chardev_test_cdev = cdev_alloc();
    cdev_init(chardev_test_cdev, &chardev_test_fops);
    int res1 = cdev_add(chardev_test_cdev ,dev ,MIN_NUM_DEV_REQ);

    if(res1 == 0){
        printk(KERN_ALERT "DEVICE kernel registration SUCCESS"); 
    }else{
        printk(KERN_ALERT "DEVICE kernel registraion FAILED"); 
    }


    /* Create a class : appears at /sys/class */
    chardev_test_class = class_create(THIS_MODULE, "chardev_test");
    /* Create a device file in /dev */
    device_create(chardev_test_class, NULL, dev, NULL, "chardev_test1");
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


