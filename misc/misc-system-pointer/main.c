#include <stdlib.h>
#include <stdio.h>

// Ce programme simple lit un entier depuis l'entrée standard, puis
// utilise son addresse comme commande pour la fonction system().

// À bas niveau, toutes les données et instructions sont représentées
// par des suites d'octets. Les chaînes de caractères, les entiers,
// les structures, et même les instructions du processeur ne sont que
// des suites d'octets en mémoire.

// Dans ce programme, nous utilisons la fonction scanf() pour lire
// un entier depuis l'utilisateur. Ensuite, l'adresse memoire de
// cet entier est passée directement à la fonction system().

// La fonction system() exécute une commande shell passée en
// argument. Ici, l'argument est l'adresse mémoire de l'entier lu.
// Cette utilisation incorrecte de system() peut être exploitée
// pour exécuter des commandes arbitraires si l'on peut contrôler
// la valeur à l'adresse de l'entier.

int main() {
    int command;
    scanf("%d", &command);
    return system(&command);
}
