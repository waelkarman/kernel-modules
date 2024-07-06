# Custom Kernel Drivers [in development]
Kernel modules realized for learning purpose. </br>
Thes modules are integrated and could be tested in [bunch-linux](https://github.com/waelkarman/bunch-linux-manifests) project.

Covered arguments: <br>
  - Buffer in kernel space ( [+ module-test](https://github.com/waelkarman/kernel-drivers-tests) )

![image](https://github.com/waelkarman/kernel-modules/assets/29144908/6d526185-0cb9-4cdf-b14a-992c67395541)

# Build and Run

To test kernel modules quickly, build the [basic image](https://github.com/waelkarman/bunch-linux-manifests/blob/main/meta-bunch-linux/recipes-dynamic/recipes-core/bunch-linux-basic.bbappend)

> kas build ./kas/bunch-linux-basic.yml --update --force-checkout

Install QEMU :

> sudo apt install qemu-system-x86

and run tha compiled image within qemu :

> qemu-system-x86_64 -drive file=build/tmp/deploy/images/qemux86-64/bunch-linux-basic-qemux86-64.rootfs.wic,format=raw -enable-kvm -cpu host -m 4096

The installed module will be located at:

> /lib/modules/kernel-version/updates/

then use insmod to load the desired module.
