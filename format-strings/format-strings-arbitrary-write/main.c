#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Liens utiles pour comprendre le format %n de printf:
//   https://www.educative.io/answers/what-is-the-use-of-n-in-printf
//   https://bases-hacking.org/ecrire-memoire.html
//
// Le format %n de printf est moins connu mais très utile pour l'exploitation
// binaire. Contrairement aux autres formats qui lisent et affichent des données,
// %n permet d'écrire en mémoire. Il s'attend à un pointeur sur un int et y écrit
// le nombre d'octets affichés par printf jusqu'à ce point.
//
// Par exemple, avec "blabla%12$n", printf interprétera le 12ème argument
// comme un pointeur. Il écrira à l'adresse pointée par cet argument la valeur
// correspondant au nombre d'octets écrits jusque-là (ici 6, soit "\x06\x00\x00\x00").
//
// Si l'adresse n'est pas valide ou ne dispose pas des droits d'écriture,
// un segfault se produira car printf tentera d'écrire à une adresse incorrecte.
//
// NOTE: %n se décline en plusieurs variantes pour écrire différents types
// d'entiers :
// %hhn : char
// %hn  : short
// %n   : int
// %ln  : long
// %lln : long long

int userid = 0;

int main() {
    // ici nous voyons a quoi sert %n a la base:
    int i, j;
    printf("Bonjour %s.\n%nAu revoir !\n%n", "etudiant", &i, &j);
    printf("DEBUG: Apres avoir dit bonjour, printf avait ecrit %d bytes\n", i);
    printf("DEBUG: Apres avoir dit au revoir, printf avait ecrit %d bytes\n", j);

    // ici nous allons tenter d'exploiter cela a notre avantage:
    int *intptr = &userid;
    char buf[64];
    printf("Votre payload: ");
    scanf("%64s", buf);
    printf(buf);
    if (userid == 42)
        system("sh");
}
