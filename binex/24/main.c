#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// ASLR et NX activés... mais ici, nous pouvons faire quelque chose de
// beaucoup plus simple qu'un shellcode : il suffit d'écraser l'adresse
// de retour avec celle d'une fonction qui nous intéresse :)

// NX (No-eXecute):
// La protection NX rend la pile (stack) non exécutable, ce qui signifie
// que nous ne pouvons pas y exécuter de shellcode. NX empêche donc
// l'exécution directe de code injecté dans la pile, rendant les attaques
// plus difficiles.

// ASLR (Address Space Layout Randomization):
// L'ASLR complique l'exploitation en randomisant les adresses de mémoire,
// mais ici, l'adresse de la fonction cible est toujours mappée à la même
// adresse, car l'ASLR ne randomise pas les pages mémoire directement
// liées à l'exécutable, comme le segment .text. Seules les pages non
// mappées sur l'exécutable, comme la libc, la pile (stack), ou le tas
// (heap), sont randomisées.

// Alignement de la pile:
// Il est crucial de veiller à l'alignement de la pile sur 16 octets
// (bytes). Le code compilé s'attend à ce que la pile soit alignée de
// cette manière pour que certaines instructions (comme celles utilisant
// les registres xmm) puissent fonctionner correctement. Si la pile n'est
// pas correctement alignée, cela peut provoquer des erreurs de
// segmentation.

void login() {
    system("/bin/sh");
}

int main() {
    char buffer[64];

    printf("Veuillez entrer votre mot de passe:\n");
    gets(buffer);
    return 0;
}
