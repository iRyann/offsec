#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Liens pour mieux comprendre les format strings et la lecture mémoire :
//  https://bases-hacking.org/format-strings.html
//  https://bases-hacking.org/lire-memoire.html
//
// La fonction printf utilise des format strings pour formater la sortie.
// Exemple : printf("Bonjour, %s, vous avez %d ans.\n", username, age),
// affichera "Bonjour, mathieu, vous avez 18 ans.\n".
// printf est une fonction à arguments variables qui détermine le nombre
// d'arguments à partir des spécificateurs de format dans la chaîne passée
// en premier argument.
//
// Lors de l'exécution de printf, la chaîne formatée est créée en fonction
// des spécificateurs. Par exemple, "%s" indique que l'argument suivant doit
// être traité comme une chaîne de caractères. De même, "%d" formate l'argument
// suivant comme un entier.
//
// Comme vu dans l'exercice ret2sc-tuto, printf dépend de la chaîne de format pour
// déterminer combien d'arguments sont passés. L'ABI System V spécifie comment
// les arguments sont passés (les 6 premiers via registres, puis sur la stack).
// Révisez ret2sc-tuto pour bien comprendre cette notion.
//
// Maintenant, que se passe-t-il si une variable contrôlée par l'utilisateur
// est passée en premier argument de printf ? Cela peut permettre de contrôler
// le nombre d'arguments affichés et leur format. Les formats les plus connus
// sont %s et %d, mais il en existe beaucoup d'autres.
//
// Pour plus de détails sur les spécificateurs de format printf, voir :
//  * https://cplusplus.com/reference/cstdio/printf/
//  * https://opensource.apple.com/source/gcc3/gcc3-1161/gcc/testsuite/gcc.dg/format/c99-printf-1.c.auto.html
//
// En exploitation native, les formats %p (ou %lx) sont souvent utilisés pour
// divulguer des informations de la stack, car ils affichent des valeurs
// sous forme hexadécimale. %s affiche une chaîne de caractères en mémoire,
// jusqu'au premier nullbyte.
//
// Dans cet exemple, une variable locale 'secret' sur la stack contient un long
// aléatoire. Le but est de trouver cette valeur pour passer le niveau.
// Comme 'username' est passé directement à printf, il est possible d'utiliser
// des %p pour divulguer des informations de la stack.

void getstr(char *b) {
    while((*b = getchar()) && *b != '\n') b++; *b = '\0';
}

int main() {
    srand(time(0));
    long secret = rand() + rand() + rand();
    char buf[64];

    printf("\nRenseignez votre username: ");
    getstr(buf);
    printf("Bienvenue, ");
    printf(buf);

    printf("\nRenseignez votre code secret: ");
    long input;
    scanf("%ld", &input);
    if (input == secret)
        system("sh");
}
