#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Attaque ROP (Return Oriented Programming)
//
// Avant de commencer, résolvez les exercices suivants, qui sont des formes
// basiques de ROP :
//      ret2func2
//      ret2plt
//
// Ces deux exercices sont des formes simples de ROP, car l'on écrase
// l'adresse de retour, non pas vers un shellcode sur la pile, mais sur du code
// légitime déjà présent dans le segment .text du binaire.
// Le ROP exploite les binaires avec la protection NX activée, qui empêche
// l'exécution de code sur la pile ou le tas (heap).
//
// Voyons maintenant comment utiliser les gadgets ROP pour exécuter du code
// arbitraire sans shellcode conventionnel.
//
// Si nous pouvons écraser l'adresse de retour, ainsi que les pointeurs suivants
// sur la pile, nous pouvons contrôler le flux d'exécution en utilisant des
// 'gadgets ROP'. Ce sont de petites séquences d'instructions qui se terminent
// par une instruction 'ret'.
//
// L'instruction 'ret' équivaut à un 'pop rip', c'est-à-dire qu'elle permet de
// sauter à l'instruction située à l'adresse sur le sommet de la pile. Ainsi,
// un gadget est exécuté, puis le 'ret' permet de sauter au gadget suivant, dont
// l'adresse a été placée sur la pile.
//
// Dans ce challenge, nous avons la fonction `system()` dans la PLT. Il suffit
// de faire en sorte que `system()` soit appelé avec "sh" comme premier
// argument. Nous devons donc trouver un gadget qui place un pointeur vers
// la chaîne "sh" dans le registre rdi (premier argument selon l'ABI x86-64),
// pour que `system("sh")` soit exécuté.
//
// Il devrait exister un endroit en mémoire contenant "sh\0" (regardez le
// premier `printf`).
//
// N'oubliez pas l'importance de l'alignement de la pile. Le gadget 'ret' est
// souvent utile dans ce contexte.

int main() {
    char name[32];

    printf("Please don't override the buffer ! Otherwise the program will crash");
    printf("\nEnter your name: ");
    gets(name);
    printf("Welcome, %s ! Here is your command:\n", name);
    system("ls -lah *");
}
