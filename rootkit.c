#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include "rk_net.h"

MODULE_LICENSE("GPL");

static int __init rootkit_init(void)
{
    rk_net_init();
    printk(KERN_INFO "Hello World\n");
    return 0;
}

static void __exit rootkit_exit(void)
{
    rk_net_exit();
    printk(KERN_INFO "GoodBye World\n");
}

module_init(rootkit_init);
module_exit(rootkit_exit);