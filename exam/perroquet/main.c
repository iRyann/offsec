#include <stdio.h>

void cat() {
    char buf[64];
    int i = 0;
    while (++i <= 2) {
        printf("Input %d: ", i);
        fgets(buf, 0x64, stdin);
        printf("Output %d: ", i);
        printf(buf);
    };
}

int main() {
    cat();
}

