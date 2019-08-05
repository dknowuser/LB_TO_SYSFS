PWD := $(shell pwd)
obj-m += lbfeat-to-sysfs.o
subdir-ccflags-y += -I$(srctree)/drivers/fpga/stcmtk/grif/grif_fpga_mgr/

all:
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C $(KERNEL) M=$(PWD) modules
clean:
	make -C $(KERNEL) M=$(PWD) clean
