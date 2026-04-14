#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int is_safe(char *cmd) {
    while (*cmd) {
        if (strchr("abcdefghijkmnopqrtuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", *cmd))
            return 0;
        cmd++;
    }
    return 1;
}

int main() {
    long command;
    scanf("%ld", &command);
    if (command < 100 && is_safe(&command))
        return system(&command);
    else
        printf("Invalid command !\n");
}

