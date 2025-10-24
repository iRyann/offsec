#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>

// CWE-190: Integer Overflow or Wraparound

int main() {
    int num;
    printf("Entrez votre code secret: ");
    scanf("%d", &num);
    num = num + num + num;
    if (num == 2)
        system("sh");
}
