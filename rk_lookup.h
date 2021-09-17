#pragma once
#include <linux/kprobes.h>
static struct kprobe kp = {.symbol_name = "kallsyms_lookup_name"};

typedef unsigned long (*rk_kallsyms_lookup_name_t)(const char *name);
rk_kallsyms_lookup_name_t rk_kallsyms_lookup_name = NULL;
unsigned long rk_lookup(const char *name) {
  if (rk_kallsyms_lookup_name == NULL) {
    register_kprobe(&kp);
    rk_kallsyms_lookup_name = (rk_kallsyms_lookup_name_t)kp.addr;
    unregister_kprobe(&kp);
  }
  return rk_kallsyms_lookup_name(name);
}