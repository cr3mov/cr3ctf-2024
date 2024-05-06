// gcc -w -o packet-maker packet-maker.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <stddef.h>
#include <sys/syscall.h>
#include <signal.h>

typedef struct packet {
    unsigned long long size;
    unsigned long long checksum;
    struct packet *next;
    struct packet *prev;
    char data[0x68];
} note;
char *protocolName[6];
note *head;


unsigned long long getChoice() {
    char buf[0x10] = {0, };
    read(0, buf, 0xF);
    return atol(buf);
}

void title() {
    puts("______             _          _    ___  ___        _               ");
    puts("| ___ \\           | |        | |   |  \\/  |       | |              ");
    puts("| |_/ /__ _   ___ | | __ ___ | |_  | .  . |  __ _ | | __ ___  _ __ ");
    puts("|  __// _` | / __|| |/ // _ \\| __| | |\\/| | / _` || |/ // _ \\| '__|");
    puts("| |  | (_| || (__ |   <|  __/| |_  | |  | || (_| ||   <|  __/| |   ");
    puts("\\_|   \\__,_| \\___||_|\\_\\\\___| \\__| \\_|  |_/ \\__,_||_|\\_\\\\___||_|   ");
    puts("                                                                   ");
}

void menu() {
    puts("1. Add Protocol");
    puts("2. Add Packet");
    puts("3. Delete Packet");
    printf("> ");
}

uint32_t compute_checksum(note *n) {
    uint64_t checksum = 0;
    uint32_t *ptr = (uint32_t *)n;

    unsigned long long words = n->size / 4;
    for (unsigned long long i = 0; i < words + 8; i++) { // 8 (size, checksum, next, prev)
        checksum += ptr[i];
        if (checksum >= 0x100000000) {
            checksum &= 0xFFFFFFFF;
            checksum++;
        }
    }

    checksum = ~checksum + 1;
    return checksum;
}

void showProtocol() {
    for (int i=0; i<6; ++i)
        if (protocolName[i])
            printf("[%d] %s\n", i + 1, protocolName[i]);
}

void addPacket() {
    unsigned long long size = 0;
    unsigned long long index = 0;
    char buf[0x88];
    note *newNote = 0;
    note *lastNote;

    printf("Size > ");
    size = getChoice();
    if (size > sizeof(newNote->data)) {
        puts("Size is too large :<");
        return;
    }

    newNote = (note *)malloc(sizeof(note));
    if (!newNote) {
        puts("Alloc failed :p");
        return;
    }

    memset(newNote, 0, sizeof(note));

    showProtocol();    
    printf("Protocol > ");
    index = getChoice() - 1;
    if (index > 5) {
        puts("Invalid Index :<");
        return;
    }

    printf("Packet > ");
    read(0, buf, size);
    newNote->size = size;
    newNote->data[snprintf(newNote->data, size, "%s:%s", protocolName[index], buf)] = '\0';

    if (head) {
        for (lastNote = head; lastNote->next && (lastNote->next != newNote); lastNote = lastNote->next);
        lastNote->next = newNote;
        newNote->prev = lastNote;
    }
    else {
        head = newNote;
    }

    newNote->checksum = compute_checksum(newNote);
    printf("checksum : %p\n", newNote->checksum);
}

void deletePacket() {
    unsigned long long index = 0;
    unsigned long long cnt = 0;
    note *lastNote;
    note *prevNote;
    
    printf("Index > ");
    index = getChoice();
    for (lastNote = head; lastNote && cnt < index; cnt++, lastNote = lastNote->next) {
        if (lastNote->next == NULL)
            break;
    }

    if (!lastNote) {
        printf("No Packet Available\n");
        return;
    }

    if (cnt != index) {
        printf("Invalid Index :<\n");
        return;
    }

    if (!index) {
        head = head->next;
        if (head)
            head->prev = NULL;
    }        
    else {
        prevNote = lastNote->prev;
        prevNote->next = lastNote->next;
    }

    free(lastNote);
}

void showChecksum() {
    note *curNote;
    int cnt = 1;
    for (curNote = head; curNote; curNote = curNote->next) {
        printf("[%d] %p\n", cnt, curNote->checksum);
        cnt++;
    }
}

void addProtocol_(const char *s) {
    int i;
    char *ptr;
    for (i=0; i<6; i++)
        if (protocolName[i] == NULL)
            break;
    
    ptr = strdup(s);
    protocolName[i] = ptr;
}

void addProtocol() {
    char buf[0x10];
    int n = 0;
    int i;

    memset(buf, 0, 0x10);
    puts("Make Your own Protocol");
    printf("> ");

    n = read(0, buf, 0xf);
    if (n <= 0) { 
        puts("read Error :(");
        exit(1);
    }

    addProtocol_(buf);
}

void setSeccomp() {
    struct sock_filter filter[] = {
        // 로드 arch 값을 검사
        BPF_STMT(BPF_LD + BPF_W + BPF_ABS, (offsetof(struct seccomp_data, arch))),
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, AUDIT_ARCH_X86_64, 1, 0),
        BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_KILL),

        // 로드 syscall 번호
        BPF_STMT(BPF_LD + BPF_W + BPF_ABS, (offsetof(struct seccomp_data, nr))),

        // mprotect 검사
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, SYS_mprotect, 0, 1),
        BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_ALLOW),

        // open 검사
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, SYS_open, 0, 1),
        BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_ALLOW),

        // read 검사
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, SYS_read, 0, 1),
        BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_ALLOW),

        // write 검사
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, SYS_write, 0, 1),
        BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_ALLOW),

        // exit 검사
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, SYS_exit, 0, 1),
        BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_ALLOW),

        // exit_group 검사
        BPF_JUMP(BPF_JMP + BPF_JEQ + BPF_K, SYS_exit_group, 0, 1),
        BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_ALLOW),

        // 그 외 모든 시스템 호출 차단
        BPF_STMT(BPF_RET + BPF_K, SECCOMP_RET_KILL),
    };

    struct sock_fprog prog = {
        .len = (unsigned short)(sizeof(filter)/sizeof(filter[0])),
        .filter = filter,
    };

    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
        perror("PR_SET_NO_NEW_PRIVS");
        exit(1);
    }

    if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog)) {
        perror("seccomp");
        exit(1);
    }
}

void init() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    addProtocol_("HTTP");
    addProtocol_("DNS");
    addProtocol_("SSH");
    setSeccomp();
}

void hint() {
    puts("I have a gift for you in the main function! I hope it helps you a lot.");
}

int main() {
    unsigned long long present = &present;
    init();
    title();
    while (1) {
        menu();
        switch(getChoice()) {
            case 1:
                addProtocol();
                break;
            case 2:
                addPacket();
                break;
            case 3:
                deletePacket();
                break;
            default:
                puts("Invalid Choice :(");
        }
    }
}
