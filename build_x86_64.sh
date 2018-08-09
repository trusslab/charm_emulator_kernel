 #!/bin/bash
#compile device tree for charm
dtc -I dts -O dtb arch/x86/kernel/charm.dts > arch/x86/kernel/charm.dtb
xxd -i arch/x86/kernel/charm.dtb > arch/x86/kernel/charm_dtb.h

#compile the kernel
export CROSS_COMPILE=x86_64-linux-android-
export ARCH=x86_64
export PATH=$PATH:/home/charm/Hamid/x86_64-linux-android-4.9/bin
make x86_64_ranchu_defconfig
make -j30
