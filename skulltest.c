#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wael Karman");
MODULE_DESCRIPTION("A Simple Hello World module");

static int param1 = 1;
static char* param2 = "2";
module_param(param1, int, S_IRUGO); //last input is a permission parameter defined in linux/stat.h
module_param(param2, charp, S_IRUGO); //can be used even the function:  module_param_array();

dev_t dev;


/*DEFINE VERBS FOR FILE OPERATIONS*/
loff_t skulltest_llseek(struct file *file, loff_t offset, int whence){
    loff_t retval = -EINVAL;
    return retval;
}

static ssize_t skulltest_read(struct file *file, char __user *buf, size_t count, loff_t *ppos){
    ssize_t ret = 0;
    return ret;
}

static ssize_t skulltest_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos){
    ssize_t ret = 0;
    return ret;
}

static long skulltest_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long __arg){
    return 0;
}

static int skulltest_open(struct inode *inode, struct file *file){
    return 0;
}

static int skulltest_release(struct inode *inode, struct file *file){
    return 0;
}


const struct file_operations skulltest_fops = {
	.owner	 = THIS_MODULE,
	.llseek	 = skulltest_llseek,
	.read	 = skulltest_read,
	.write	 = skulltest_write,
	.compat_ioctl   = skulltest_compat_ioctl,
    .open	 = skulltest_open,
	.release = skulltest_release,
};


struct cdev* skulltest_cdev = NULL;

/*DEFINE LOADING-UNLOADING BEHAVIOUR */
static int __init skulltest_init(void)
{
    int res0 = alloc_chrdev_region(&dev,0,5, "skulltest");
    if(res0 == 0){
        printk(KERN_ALERT "DEVICE DRIVER registraion success");    
    }else{
        printk(KERN_ALERT "REGISTRATION FAILED");
    }

    skulltest_cdev = cdev_alloc();
    cdev_init(skulltest_cdev, &skulltest_fops);
    skulltest_cdev->owner = THIS_MODULE;
    skulltest_cdev->ops = &skulltest_fops;
    int res1 = cdev_add(skulltest_cdev ,dev ,1);
    if(res1 == 0){
        printk(KERN_ALERT "DEVICE FILE registration success"); 
    }else{
        printk(KERN_ALERT "DEVICE FILE registraion success"); 
    }
    printk(KERN_ALERT "Hello world thats a skulltest! %x %x , COMM: %s ,PID: %i \n",param1,param2,current->comm, current->pid);
    return 0;    
}

static void __exit skulltest_cleanup(void)
{
    unregister_chrdev_region(dev, 5);
    cdev_del(skulltest_cdev);
    printk(KERN_ALERT "Cleaning up module.\n");
}

module_init(skulltest_init);
module_exit(skulltest_cleanup);



/*EXPORT A SYMBOT TO USE IT FROM APPLICATIONS AND OTHER MODULES*/
int exp_func(int i)
{
    pr_info("%s:%d the value passed in is %d\n",
            __func__, __LINE__, i);

    return i;
}
EXPORT_SYMBOL(exp_func);


