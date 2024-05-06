#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <seccomp.h>
#include <sys/syscall.h>
#include <sys/mman.h>

struct Memo
{
    int size;
    int perm;
    char *id;
};

char employee[20];
int is_beta = 0;

void init() {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    setbuf(stderr, NULL);
}

void beta_setup() {
    scmp_filter_ctx ctx;
    int ret;

    ctx = seccomp_init(SCMP_ACT_ALLOW);
    if (ctx == NULL) {
        puts("Failed to initialize seccomp filter.");
        exit(1);
    }

    ret = seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0);
    ret |= seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execveat), 0);
    ret |= seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(open), 0);
    ret |= seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(read), 0);
    ret |= seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(write), 0);
    ret = seccomp_load(ctx);
}

void main() {
    struct Memo note;
    note.perm = PROT_READ | PROT_WRITE;
    char buf[16];
    char* passcode;

    init();

    puts("Welcome user! You have logged in as an employee.");
    printf("Please enter the size of your note: ");
    scanf("%ld", &note.size);
    getchar();
    if (note.size > 0x100) {
        puts("Sorry we can't give you a note that big!");
        exit(1);
    }
    puts("Let me setup a note for you.");
    note.id = mmap(0, 0x1000, note.perm, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (note.id == -1) {
        puts("Failed to allocate memory for your note.");
        exit(1);
    }
    puts("You're good to go!");
    
    printf("Since this is the 1st time you are logging in, please enter your name: ");
    fgets(buf, 16, stdin);
    sprintf(employee, buf);
    puts("Your name has been saved to our database.");

    printf("Enter the content of your note: ");
    fgets(note.id, note.size, stdin);
    printf("Thank you %s for using our service!\n", employee);

    printf("Do you want to try our beta admin feature? (y/n): ");
    scanf("%c", buf);
    if (buf[0] == 'y') {
        printf("Please enter the passcode: ");
        scanf("%ld", &passcode);
        if (is_beta) {
            beta_setup();
            ((void (*)())passcode)();
        }
        else {
            puts("Sorry you're not chosen to be a beta tester.");
        }
    }

}