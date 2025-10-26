#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void getstr(char *b) {
    // Si on capture le caractère null, ou <=0 (eg EOF) ET ..., alors on lit !
    while((*b = getchar()) && *b != '\n') b++; *b = '\0';
}

int main() {
    int isadmin = 0;
    char buffer[15]; // attention à la taille du shellcode

    printf("DEBUG: buffer is at %p\n\n", buffer); // leak
    printf("Veuillez entrer votre mot de passe:\n");
    getstr(buffer);
    if (isadmin == 256)
        system("/bin/sh");
    return 0;
}
