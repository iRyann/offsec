#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void getstr(char *b) {
    while((*b = getchar()) && *b != '\n') b++; *b = '\0';
}

int main() {
    int isadmin = 0;
    long custom_canary = 0xdeadbeefdeadbeef;
    char buffer[15];

    printf("DEBUG: buffer is at %p\n\n", buffer);
    printf("Veuillez entrer votre mot de passe:\n");
    getstr(buffer);
    if (isadmin == 256)
        system("/bin/sh");
    if (custom_canary != 0xdeadbeefdeadbeef)
        exit(1);
    return 0;
}
