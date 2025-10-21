#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <time.h>

//   https://docs.pwntools.com/en/stable/
//   https://github.com/Gallopsled/pwntools-tutorial
//   https://gist.github.com/anvbis/64907e4f90974c4bdd930baeb705dedf
//   Google
//   Stack Overflow


void calcul_mental() {
    int a = rand() % 30000;
    int b = rand() % 30000;
    int op = rand() % 2;
    int answer;

    if (op == 0) {
        printf("Combien font %d fois %d ?\n", a, b);
        scanf("%d", &answer);
        if (a*b != answer)
            exit(0);
    } else {
        printf("Combien font %d plus %d ?\n", a, b);
        scanf("%d", &answer);
        if (a+b != answer)
            exit(0);
    }
}

int main() {
    printf("Utilisez pwnlib (ou pas) pour resoudre ce challenge avant le timeout !\n");

    srand(time(NULL));
    for (int i=0; i<40; i++)
        calcul_mental();

    printf("Voici votre shell:\n");
    return system("/bin/sh");
}
