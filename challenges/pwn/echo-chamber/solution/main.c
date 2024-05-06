// gcc main.c -o chal -m32 -Wl,-z,norelro -no-pie

#include <stdio.h>
#include <string.h>

void init() {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    setbuf(stderr, NULL);
}

void filter(char* buf, int len) {
    long i;
    for(i = 0; i < len; i++) {
        switch(buf[i])
        {
            case 'a':
            case 'A':
            case 'b':
            case 'B':
            case 'd':
            case 'e':
            case 'E':
            case 'f':
            case 'F':
            case 'g':
            case 'G':
            case 'i':
            case 'o':
            case 'p':
            case 's':
            case 'u':
            case 'x':
            case 'X':
                puts(buf);
                return;
            default:
                continue;
        }
    }
    printf(buf);
}

void main() {
    init();
    char buf[72];
    int len = 0;
    puts("Welcome to my echo chamber!");
    write(1, "Shout your message: ", 20);
    len = read(0, buf, 72);
    buf[len] = 0;
    puts("Your message is:");
    filter(buf, len);
}