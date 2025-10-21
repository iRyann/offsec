#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int userid = 0;
    char username[115];

    printf("User ID: ");
    scanf("%d", &userid);

    if (userid < 1000) {
        printf("Username: ");
        gets(username);
    } else {
        printf("Your User ID (%d) can't be > 1000");
    }

    if (!strcmp(username, "admin") && userid == 1000)
        system("/bin/sh");
    return 0;
}
