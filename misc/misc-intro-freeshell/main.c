#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Lisez et comprenez le code source

char *logged_in_user = NULL;

char *read_user_input() {
    char input[16];
    printf("\n"
           "Available commands:\n"
           "  login <username>:  Login as an user\n"
           "  exec <cmd>:        Execute a command\n"
           "\n"
           "CMD> ");
    return fgets(input, 16, stdin);
}

void login(char *arg) {
    int i=0, j=0;
    char username[16];
    do { // remove spaces & newlines from username
        if (arg[i] != ' ' && arg[i] != '\n')
            username[j++] = arg[i];
    } while (arg[i++]);

    printf("You are now logged-in as %s\n", username);
    logged_in_user = username;
}

void exec(char *arg) {
    if (logged_in_user)
        system(arg);
    else
        printf("You are NOT logged-in !\n");
}

int main() {
    int num_commands = 2;

    while (num_commands--) {
        char *input = read_user_input();
        if (!strncmp(input, "login ", 5))
            login(input+6);
        if (!strncmp(input, "exec ", 4))
            exec(input+5);
    }
    printf("You have no commands left !\n");
    return 0;
}
