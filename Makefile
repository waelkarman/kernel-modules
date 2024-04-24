obj-m = chardev_test.o

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) 
clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) clean
modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules_install
