#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// avec un canary fixe

int main() {
    int isadmin = 0;
    long custom_canary = 0xcafeCAFEcafeCAFE;
    char buffer[15];

    printf("DEBUG: buffer is at %p\n\n", buffer);

    printf("Veuillez entrer votre mot de passe:\n");
    gets(buffer);
    if (custom_canary != 0xCAFEcafeCAFEcafe)
        exit(1);
    return 0;
}
