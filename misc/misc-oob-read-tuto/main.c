#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <stddef.h>
#include <time.h>

// CWE-125: Out-of-bounds Read
// https://cwe.mitre.org/data/definitions/125.html

// Quand on indexe un tableau, une arithmetique simple est faite sur le pointeur.
// Prenons un tableau T, de type pointeur sur int (int*), et un index N.
//  * Un int fait 4 bytes (sizeof(int) === 4) sur cette architecture.
// Appeler T[N] correspond a faire *(T + ( N * sizeof(int) ) ).
// Autrement dit, recuperer le Nieme entier sur le tableau.
//
// Exemple:
//   Un tableau ainsi declare:
//      int T[3]  = { 0xaaaaaaaa, 0xbbbbbbbb, 0xcccccccc },
//   En supposant que mon tableau soit stocke en memoire a l'adresse 0x1000
//   correspond a une zone memoire de 12 bytes (3 * sizeof(int)), dont voici un hexdump:
//      AA AA AA AA BB BB BB BB CC CC CC CC
//
//   Si je veux acceder au troisieme int (en utilisant T[2]), je dois donc lire
//   4 bytes a l'adresse 0x1008, pour lire les bytes `CC CC CC CC`
//
//   Voila aussi pourquoi "password"[1] renvoie 'a', car dans ce cas on recupere
//   1 byte (char), a l'adresse de "password", plus 1 fois sizeof(char) (donc 1)
//   Donc si ce tableau char* etait stocke d l'adresse 0x2000, demander l'index
//   1 correspond a recuperer le byte stocke d l'adresse 0x2001 (donc 'a')
//   L'index est donc toujours multiplie par la taille de l'element (sizeof(elem)),
//   puis additionne au pointeur, pour finalement 'lire' sizeof(elem) bytes.
//
// Alors que se passe-t-il si un attaquant peut controler un index mal defini,
// permettant une lecture ou ecriture out of bounds ?
// Si cela se produit, il est possible de lire (ou ecrire selon le type de vuln)
// en dehors du tableau pour lequel etait prevue l'operation.
//
// Si pour mon meme tableau T, je suis en mesure de demander T[1000], j'accede
// aux 4 bytes situes a l'adresse 0x1000 + (1000 * sizeof(int)), qui peuvent
// contenir des informations sensibles... Pour couronner le tout, cela fonctionne
// egalement avec des indices NEGATIFS...
//
// Comme d'habitude, concevez d'abbord votre exploit en local. Je conseille de creer
// vous-meme un fichier flag.txt dans votre repertoire pour tester en local, avant
// de lancer l'exploit en remote.

void *get_admin_password() {
    char *pass;
    char buf[256];
    for (int i=0; i<16; i++)
        buf[i] = (char)((rand() % 26) + 0x41);
    buf[16] = 0;
    pass = strdup(buf);
    return pass;
}

int main(int argc, char **argv) {
    srand(time(NULL));
    char *password = get_admin_password();
    char buf[256];
    int mois;
    char *zodiaque[] = { NULL, "Capricorne", "Verseau", "Poissons", "Belier", "Taureau", "Gemeaux", "Cancer", "Lion", "Vierge", "Balance", "Scorpion", "Sagittaire" };

    printf("Super horoscope !\nDonnez-moi votre mois de naissance, je vous dirais quel est votre signe:\n");
    scanf("%d", &mois);
    if (mois > sizeof(zodiaque))
        exit(printf("%d n'est pas un mois valide !\n", mois));
    printf("Vous etes du signe '%s' !\n", zodiaque[mois]);

    printf("Si vous etes astrologue professionnel, entrez votre mot de passe:\n");
    scanf("%16s", buf);
    if (!strcmp(buf, password))
        system("sh");
}
