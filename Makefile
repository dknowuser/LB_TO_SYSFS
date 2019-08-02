PWD := $(shell pwd)
obj-m += lbfeat-to-sysfs.o

all:
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C $(KERNEL) M=$(PWD) modules
clean:
	make -C $(KERNEL) M=$(PWD) clean
