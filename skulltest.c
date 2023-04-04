#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wael Karman");
MODULE_DESCRIPTION("A Simple Hello World module");

static int param1 = 1;
static char* param2 = "2";
module_param(param1, int, S_IRUGO); //last input is a permission parameter defined in linux/stat.h
module_param(param2, charp, S_IRUGO); //can be used even the function:  module_param_array();

dev_t dev;

static int __init skulltest_init(void)
{
    int res = alloc_chrdev_region(&dev,0,5, "skulltest");
    if(res == 0){
        printk(KERN_ALERT "DEVICE DRIVER registraion success");    
    }else{
        printk(KERN_ALERT "REGISTRATION FAILED");
    }
    printk(KERN_ALERT "Hello world thats a skulltest! %x %x , COMM: %s ,PID: %i \n",param1,param2,current->comm, current->pid);
    return 0;    
}

static void __exit skulltest_cleanup(void)
{
    unregister_chrdev_region(dev, 5);
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



struct file_operations skulltest_fops = {
	.owner	 = THIS_MODULE,
	.llseek	 = skulltest_llseek,
	.read	 = skulltest_read,
	.write	 = skulltest_write,
	.ioctl   = skulltest_ioctl,
    .open	 = skulltest_open,
	.release = skulltest_release,
};