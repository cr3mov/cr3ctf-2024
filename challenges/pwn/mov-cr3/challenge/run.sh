#!/bin/sh
qemu-system-x86_64 \
    -m 64M \
    -kernel /app/bzImage \
    -initrd /app/rootfs.cpio \
    -append "root=/dev/ram rw console=ttyS0 oops=panic panic=1 quiet kaslr" \
    -nographic \
    -cpu qemu64,smep,smap \
    -no-reboot
