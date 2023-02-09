obj-m = hello.o
KVERSION = $(shell uname -r)
all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
modules_install:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) 
