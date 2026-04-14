#include <stdio.h>
#include <stdlib.h>

int main() {
    int (*display)(const char *) = puts;
    char username[64];

    // input
    printf("Your username: ");
    gets(username);

    // output
    system("echo 'Your username is:'");
    return display(username);
}
