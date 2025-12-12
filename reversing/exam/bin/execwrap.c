#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv, char **envp) {
    if (argc < 2) {
    }

    // argv = ["./execwrap", a1, a2, ...]
    argv[0] = (char*)"./exam";
    execve("./exam", argv, envp);
    perror("execve");
    return 1;
}

