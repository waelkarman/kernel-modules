obj-m = chardev_test.o
SRC = $(shell pwd)
KVERSION = "5.15.0-102-generic"
all:
	$(MAKE) -C /lib/modules/$(KVERSION)/build M=$(SRC) modules
clean:
	$(MAKE) -C /lib/modules/$(KVERSION)/build M=$(SRC) clean
modules_install:
	$(MAKE) -C /lib/modules/$(KVERSION)/build M=$(SRC) modules_install
