#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Illustrons la puissance du format %n dans printf.
// Comme notre buffer (buf) est sur la stack et que nous contrôlons son contenu,
// nous pouvons y écrire une adresse et utiliser %n pour écrire un entier à
// cette adresse.
//
// Par exemple, si buf commence à [RSP+8], et que le payload est:
// "\x10\x20\x40\x00\x00\x00\x00\x00%90c%7$lln", voici ce qui se passe:
//
// 1. Les 8 premiers bytes écrits dans buf (à [RSP+8]) représentent l'adresse
//    où nous voulons écrire (ici 0x402010).
// 2. Le "%90c" ajoute 90 caractères pour totaliser 98 (8 + 90) caractères écrits.
// 3. Le %7$lln écrit la valeur 98 à l'adresse stockée dans le 7ème argument de
//    printf, soit [RSP+8], donc 0x402010.
//    Cela écrit \x00\x00\x00\x00\x00\x00\x00\x62 à l'adresse 0x402010.
//
// Nous avons ainsi une écriture arbitraire à une adresse contrôlée !
// De plus, nous pouvons écrire byte par byte en modifiant notre payload.
//
// Par exemple, pour écrire 0x2e5b3eee à l'adresse 0x402010 :
// - Ecrire 0x2e à 0x402010
// - Ecrire 0x5b à 0x402011
// - Ecrire 0x3e à 0x402012
// - Ecrire 0xee à 0x402013
//
// Pour chaque byte, ajuster le padding pour que printf ait écrit exactement
// le bon nombre de caractères avant d'utiliser %n pour écrire le byte à
// la position souhaitée.
//
// Cela permet d'écraser la GOT (Global Offset Table) pour rediriger une
// fonction vers une autre, par exemple, en remplaçant l'adresse d'une
// fonction par celle de success(), qui ouvre un shell.

void success() {
    system("sh");
}

int main() {
    char buf[150];
    printf("Votre nom: ");
    scanf("%149s", buf);
    printf(buf);
    printf(", bienvenue !\n");
}
