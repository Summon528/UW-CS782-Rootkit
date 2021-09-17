#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>

MODULE_LICENSE("GPL");

static int __init rootkit_init(void)
{
    printk(KERN_INFO "Hello World\n");
    return 0;
}

static void __exit rootkit_exit(void)
{
    printk(KERN_INFO "GoodBye World\n");
}

module_init(rootkit_init);
module_exit(rootkit_exit);