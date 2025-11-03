#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Parfois, il est possible de divulguer une adresse nécessaire pour
// l'exploitation, mais le programme se termine immédiatement après,
// rendant l'exploitation difficile une fois l'adresse connue.
//
// Cependant, il peut être possible de procéder à une attaque en deux
// étapes : d'abord, le programme divulgue l'adresse, puis il s'apprête
// à quitter. Toutefois, on peut forcer le programme à se relancer, et
// cette fois, nous connaissons déjà l'adresse divulguée !
//
// L'attaque "ret2main" consiste à revenir au début du programme
// pour `rejouer` ce qui nous intéresse, mais selon les situations, il n'est
// pas toujours nécessaire de revenir jusqu'à `main`.
//
// PS : Il est préférable de bien comprendre le concept de frames de pile
// (stack frames). Il est recommandé de maîtriser les exercices
// suivants avant de poursuivre :
//      * ret2func
//      * ret2func2
//      * stackaddr-escape

void getstr(char *b) {
    while((*b = getchar()) && *b != '\n') b++; *b = '\0';
}

void show_alphabet() {
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz", lettre[] = "a";
    printf("A partir de quelle lettre voulez-vous afficher l'alphabet ? : ");
    getstr(lettre);
    printf("Voici l'alphabet, a partir de la lettre %c:\n%s\nAu revoir !\n",
            *lettre, &alphabet[*lettre-'a']);
}

int main() {
    show_alphabet();
}
