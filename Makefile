obj-m = skulltest.o
SRC = $(shell pwd)
all:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) 
modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules_install
