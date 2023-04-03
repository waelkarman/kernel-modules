#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/errno.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wael Karman");
MODULE_DESCRIPTION("A Simple Hello World module");

static int param1 = 1;
static int param2 = 2;
module_param(param1, int, S_IRUGO);
module_param(param2, int, S_IRUGO);

static int __init hello_init(void)
{
    printk(KERN_ALERT "Hello world! %x %x , COMM: %s ,PID: %i \n",param1,param2,current->comm, current->pid);
    return 0;    
}



static void __exit hello_cleanup(void)
{
    printk(KERN_ALERT "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
