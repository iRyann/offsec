#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// ressources:
//  https://bitwisecmd.com/
//  https://learningcontent.cisco.com/games/binary/index.html
//  https://www.wikihow.com/Read-Binary
//
//  visualize a stack (push/pop):
//      https://dsvisualizer.isatvik.com/stack
//      https://stack-visualize.netlify.app/
//
//  test ASM instructions:
//      https://asm.diveintosystems.org/
//
//  tuto decimal/binaire/hex:
//      https://fr.wikihow.com/convertir-un-nombre-hexad%C3%A9cimal-en-nombre-binaire-ou-en-nombre-d%C3%A9cimal

int challenge() {
    char s[128];
    long l;
    int i;

    printf("Ecrivez le long 234987 en binaire (format 01010101 (64 bits)):\n");
    gets(s);
    if ((strlen(s) != 64) || (strtol(s, NULL, 2) != 234987)) {
        return 0;
    }

    printf("Ecrivez l'int 4628 en hexadecimal (format aabbccdd):\n");
    scanf("%8x", &i);
    getchar();
    if (i != 4628) {
        return 0;
    }

    printf("Ecrivez 0x91ff en signed short:\n");
    gets(s);
    if (*s != '-' || (unsigned short)atoi(s) != 0x91ff) {
        return 0;
    }

    /* printf("Entrez la valeur hexadecimale */

    printf("Entrez un string equivalent au long 27657434811745362:\n");
    gets(s);
    if (*(long*)s != 27657434811745362) {
        return 0;
    }

    printf("Entrez un long equivalent au string 'H4ck3r':\n");
    scanf("%ld", &l);
    getchar();
    if (strcmp((char*)&l, "H4ck3r") != 0) {
        return 0;
    }

    return 1;
}

int main() {
    if (challenge())
        system("sh");

    printf("Bye!\n");
}
