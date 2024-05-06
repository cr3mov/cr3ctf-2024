#include <stdio.h>
#include <string.h>

void init() {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    setbuf(stderr, NULL);
}

void win(char c) {
    char filename[5] = {c, 'l', 'a', 'g', '\0'};
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }
    char buf[256];
    while (fgets(buf, sizeof(buf), file)) {
        printf("%s", buf);
    }
    fclose(file);
}

void menu() {
    puts("1. Store a number");
    puts("2. Exit");
    printf("> ");
}

void main() {
    init();
    puts("Welcome to the the number warehouse!");
    int num[20] = {0};
    int idx = 0;
    while (1) {
        menu();
        int choice;
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                printf("Enter an index: ");
                scanf("%d", &idx);
                getchar();
                if (idx % 2 == 0) {
                    printf("Enter a new number: ");
                    scanf("%d", &num[idx]);
                    printf("Successfully stored %d\n", num[idx]);
                }
                else {
                    puts("Forbidden!");
                }
                break;
            case 2:
                return;
            default:
                puts("Invalid choice!");
        }
    }
}