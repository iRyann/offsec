#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Nous allons manquer d'espace... il est temps d'utiliser un stack pivot !
//
// En effet, si nous contrôlons une autre zone mémoire, nous pouvons utiliser
// un "stack pivot" pour rediriger l'exécution de la ropchain.
// Le principe est de remplacer la pile actuelle (pointée par RSP) par une autre
// zone mémoire sous notre contrôle pour poursuivre l'exécution.
//
// Dans cet exemple, nous pouvons faire pivoter la pile vers la section .data.
// Cependant, rien ne vous empêche de pivoter vers la heap, la pile elle-même
// (plus haut ou plus bas), ou toute autre zone mémoire accessible en lecture et
// en écriture.
//
// Attention : quand vous appelez `system()`, le programme continue son exécution
// et crée de nouvelles stackframes pour les fonctions sous-jacentes. Il est donc
// impératif que votre "fake stack" ait suffisamment d'espace pour éviter que
// les nouvelles stackframes ne débordent sur une page en lecture seule, ce qui
// provoquerait un crash garanti.

#define EQUALS(x, y) *(long*)x == *(long*)y
#define BUF_SZ 32
char user[BUF_SZ];

void login(char *user) {
    char pass[8];
    printf("Hello %s\n", user);
    printf("Password: ");
    fgets(pass, BUF_SZ, stdin);

    if (EQUALS(user, "satoshi") && EQUALS(pass, "nakamoto")) {
        system("ls -lah *");
    } else {
        printf("Wrong credentials !\n");
    }
}

int main() {
    printf("BTC WALLET\n\n");
    printf("Username: ");
    gets(user);

    login(user);
    printf("Bye !\n");
}
