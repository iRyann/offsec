#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Cette fois, le buffer disponible pour le username est plus petit.
// Cependant, printf permet de spécifier l'argument à utiliser pour chaque
// format avec N$, où N est le numéro de l'argument.
//
// Par exemple : printf("%p %p %p") est équivalent à printf("%1$p %2$p %3$p"),
// les deux affichent le premier, le deuxième, puis le troisième argument.
//
// Pour afficher uniquement le 300ème argument, on peut utiliser %300$p.
//
// Cette fonctionnalité fonctionne avec tous les spécificateurs de format,
// tels que %s, %d, %c, %ld, etc.
//
// Ici, en plus de trouver le 'secret', il faut aussi récupérer le stack
// protector (canary). Le canary est une protection contre les buffer overflows.
// Il est généré à l'initialisation du programme et placé dans chaque stack
// frame critique, juste avant l'adresse de retour.
//
// Avant de retourner d'une fonction, le programme vérifie si le canary a été
// modifié. Si c'est le cas, le programme plante, empêchant l'exécution
// d'un shellcode. Si nous pouvons dévoiler la valeur du canary, nous pourrons
// la réécrire correctement après un overflow pour tromper le programme.

#define CANARY *(long*)(buf+0xc)

int main() {
    srand(time(0));
    long secret = rand() + rand() + rand();
    char buf[12];

    printf("\nRenseignez votre username: ");
    fgets(buf, 12, stdin);
    printf("Bienvenue, ");
    printf(buf);

    printf("\nRenseignez votre code secret: ");
    long input;
    scanf("%ld", &input);
    if (input != secret)
        exit(0);

    printf("\nRenseignez le stack canary: ");
    scanf("%ld", &input);
    if (input == CANARY)
        system("sh");
}

