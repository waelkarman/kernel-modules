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

dev_t dev = MKDEV(220,0);

static int param1 = 1;
static char* param2 = "2";
module_param(param1, int, S_IRUGO); //last input is a permission parameter defined in linux/stat.h
module_param(param2, charp, S_IRUGO);
//can be used even the function:  module_param_array();

static int __init hello_init(void)
{
    int res = alloc_chrdev_region(dev,0,5, "skulltest");
    if(res == 0){
        printk(KERN_ALERT "DEVICE DRIVER registraion success");    
    }
    printk(KERN_ALERT "Hello world! %x %x , COMM: %s ,PID: %i \n",param1,param2,current->comm, current->pid);
    return 0;    
}


/*EXPORT A SYMBOT TO USE IT FROM APPLICATIONS AND OTHER MODULES*/
int exp_func(int i)
{
    pr_info("%s:%d the value passed in is %d\n",
            __func__, __LINE__, i);

    return i;
}
EXPORT_SYMBOL(exp_func);

static void __exit hello_cleanup(void)
{
    unregister_chrdev_region(dev, 5);
    printk(KERN_ALERT "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
