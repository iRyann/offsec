#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// https://repository.root-me.org/Exploitation%20-%20Syst%C3%A8me/Unix/EN%20-%20Heap%20and%20BSS%20overflow%201.pdf

#define BUF_LEN 32
char buf[BUF_LEN] = "admin";

void get_username() {
    printf("Your username: ");
    fread(buf, sizeof(buf), BUF_LEN, stdin);
}

int has_access() {
    static int is_admin = 0;
    return is_admin == 1;
}

int main() {
    get_username();
    if (!strcmp(buf, "admin") && has_access())
        system("sh");
}
