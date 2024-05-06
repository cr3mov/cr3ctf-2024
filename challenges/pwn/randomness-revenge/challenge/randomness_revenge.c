#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <seccomp.h>
#include <fcntl.h>

unsigned long* sc;

void init(){
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    setbuf(stdin, NULL);
    sc = mmap((void*)sbrk(0), 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    sc[0] = 0x9090fa31f931f831;
    
    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_KILL);
    // Check arch
    seccomp_arch_add(ctx, SCMP_ARCH_X86_64);

    // Allow open, read, write and exit syscalls
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(openat), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit), 0);

    // Load the seccomp filter into the kernel
    seccomp_load(ctx);
}

unsigned long randnum(){
    unsigned long num = 0;
    for (int i = 0; i < 8; i++)
        num = (num << 8) | (rand() & 0xff);
    return num;
}


void main(){
    init();
    int num;
    unsigned int seed = 0;
    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, &seed, 4);
    srand(seed);

    printf("Enter a number: ");
    scanf("%d", &num);
    if (num > 511){
        printf("Too large!\n");
        exit(0);
    }

    for (int i = 0; i < num; i++){
        unsigned long key = randnum() << (8 * ((7 - i) % 8));
        printf("> ");
        scanf("%lu", &sc[i+1]);
        sc[i + 1] ^= key;
    }
    ((void(*)())sc)();
    exit(0);
}