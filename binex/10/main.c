#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Écrasez l'adresse de retour avec celle d'une fonction qui vous intéresse.

// Adresse de retour:
//   L'adresse de retour est une valeur stockée sur la pile (stack) par
//   chaque fonction appelée dans un programme. Elle indique l'emplacement
//   mémoire où le programme doit revenir après avoir terminé l'exécution
//   de la fonction en cours. Cette adresse est automatiquement sauvegardée
//   sur la pile lors de l'appel d'une fonction (instruction `call` en
//   assembleur), et est utilisée par l'instruction `ret` pour restaurer
//   le flux d'exécution à l'instruction suivant le call dans la fonction
//   appelante.
//
//   L'écrasement de l'adresse de retour est une technique utilisée dans
//   certaines attaques de dépassement de tampon (buffer overflow). Si un
//   attaquant parvient à écrire au-delà de la taille allouée d'un buffer,
//   il peut potentiellement écraser l'adresse de retour avec une valeur
//   arbitraire. En modifiant cette adresse de retour, l'attaquant peut
//   rediriger le flux d'exécution du programme vers un emplacement de son
//   choix, par exemple un shellcode injecté sur la pile.
//
//   Cela permet à l'attaquant de prendre le contrôle du programme et
//   d'exécuter un code malveillant, contournant ainsi les protections
//   mises en place par le système, telles que l'ASLR ou la protection NX.

// ASLR (Address Space Layout Randomization):
//   L'ASLR complique l'exploitation en randomisant les adresses de mémoire,
//   mais ici, l'adresse de la fonction cible est toujours mappée à la même
//   adresse, car l'ASLR ne randomise pas les pages mémoire directement
//   liées à l'exécutable, comme le segment .text. Seules les pages non
//   mappées sur l'exécutable, comme la libc, la pile (stack), ou le tas
//   (heap), sont randomisées.

void login() {
    execve("/bin/sh", NULL, NULL);
}

int main() {
    char buffer[64];

    printf("Veuillez entrer votre mot de passe:\n");
    gets(buffer);
    return 0;
}
