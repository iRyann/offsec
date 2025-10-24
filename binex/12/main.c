#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdint.h>

// Ce tutoriel explique le fonctionnement de la pile (stack) en x86_64 
// SystemV ABI. Il montre comment les arguments sont passés aux fonctions 
// et illustre les vulnérabilités de dépassement de tampon (buffer overflow).

// Note: Cette fois, le code n'exécute pas /bin/sh. Vous devrez écrire 
// un shellcode pour cela !

// Ressources :
//   https://ir0nstone.gitbook.io/notes/
//   https://www.pingidentity.com/fr/resources/cybersecurity-fundamentals/threats/buffer-overflow-attack.html
//   https://bases-hacking.org/coding-shellcode.html

int tuto() {
    char buffer[96] = "\x01\x02\x03\x04\x05\x06\x07\x08\x01\x02\x03\x04"
                      "\x05\x06\x07\x08\x01\x02\x03\x04\x05\x06\x07\x08"
                      "\x01\x02\x03\x04\x05\x06\x07\x08\x01\x02\x03\x04"
                      "\x05\x06\x07\x08\x01\x02\x03\x04\x05\x06\x07\x08"
                      "\x01\x02\x03\x04\x05\x06\x07\x08\x01\x02\x03\x04"
                      "\x05\x06\x07\x08\x01\x02\x03\x04\x05\x06\x07\x08"
                      "\x01\x02\x03\x04\x05\x06\x07\x08\x01\x02\x03\x04"
                      "\x05\x06\x07\x08\x01\x02\x03\x04\x05\x06\x07\x08";
    int int_test = 0xAABBCCDD;

    printf(
        "Dans l'ABI SystemV x86_64, les 6 premiers arguments des fonctions\n"
        "sont passés dans les registres suivants :\n"
        "  1er: RDI, 2e: RSI, 3e: RDX, 4e: RCX, 5e: R8, 6e: R9\n"
        "\n"
        "Si une fonction prend plus de 6 arguments, les suivants sont\n"
        "passés sur la stack :\n"
        "  7e: *(RSP), 8e: *(RSP+8), 9e: *(RSP+16), etc.\n"
        "\n"
        "printf() est une fonction avec un nombre variable d'arguments.\n"
        "Elle détermine combien d'arguments lire en fonction du nombre de\n"
        "placeholders ('%%') dans la chaîne de format. Si vous passez\n"
        "beaucoup de %%p sans fournir assez d'arguments, printf lira les\n"
        "registres et la stack selon l'ABI :\n"
        "\n"
        "    0x%016" PRIxPTR ": registre RSI\n"
        "    0x%016" PRIxPTR ": registre RDX\n"
        "    0x%016" PRIxPTR ": registre RCX\n"
        "    0x%016" PRIxPTR ": registre R8\n"
        "    0x%016" PRIxPTR ": registre R9\n"
        "\nNote : Le registre RDI, qui contient la chaîne de format, n'est\n"
        "pas affiché ici.\n"
        "\n"
        "  ---------- FRAME ACTUELLE DE tuto() ----------------------\n"
        "Ici, nous voyons le contenu de la frame actuelle sur la stack.\n"
        "La dernière valeur est l'adresse de retour.\n"
        "Si un dépassement de tampon se produit, cette adresse peut être\n"
        "écrasée pour rediriger l'exécution vers un shellcode placé sur\n"
        "la stack.\n"
        "\n"
        "    0x%016" PRIxPTR ": buffer (octets 0  à 7)\n"
        "    0x%016" PRIxPTR ": buffer (octets 8  à 15)\n"
        "    0x%016" PRIxPTR ": buffer (octets 16 à 23)\n"
        "    0x%016" PRIxPTR ": buffer (octets 24 à 31)\n"
        "    0x%016" PRIxPTR ": buffer (octets 32 à 39)\n"
        "    0x%016" PRIxPTR ": buffer (octets 40 à 47)\n"
        "    0x%016" PRIxPTR ": buffer (octets 48 à 55)\n"
        "    0x%016" PRIxPTR ": buffer (octets 56 à 63)\n"
        "    0x%016" PRIxPTR ": buffer (octets 64 à 71)\n"
        "    0x%016" PRIxPTR ": buffer (octets 72 à 79)\n"
        "    0x%016" PRIxPTR ": buffer (octets 80 à 87)\n"
        "    0x%016" PRIxPTR ": buffer (octets 88 à 95)\n"
        "    0x%016" PRIxPTR ": <8 OCTETS INUTILISÉS>\n"
        "    0x%016" PRIxPTR ": <4 OCTETS INUTILISÉS>, suivi de int_test\n"
        "    0x%016" PRIxPTR ": saved_RBP\n"
        "    0x%016" PRIxPTR ": RETURN_ADDR (à écraser pour un buffer overflow)\n"
        "\n"
        "  ---------- FRAME PRECEDENTE -> caller() -----------------\n"
        "Ici, nous voyons la frame de la fonction appelante (caller()).\n"
        "Cette frame contient ses variables locales et l'adresse de retour\n"
        "vers la fonction précédente.\n"
        "\n"
        "    0x%016" PRIxPTR "\n"
        "    0x%016" PRIxPTR "\n"
        "    0x%016" PRIxPTR "\n"
        "    0x%016" PRIxPTR "\n"
        "\n"
        "  ---------- FRAME ENCORE AVANT -> main() -----------------\n"
        "Plus bas dans la stack, on trouve les frames des fonctions qui\n"
        "ont appelé caller(), puis main(), avec argc, argv, envp, etc.\n"
        "    0x%016" PRIxPTR "\n"
        "    0x%016" PRIxPTR "\n"
        "    0x%016" PRIxPTR "\n"
        "    0x%016" PRIxPTR "\n"
        "    ...\n"
        "\n"
        "===================== EXPLOITATION ====================\n"
        "La fonction gets() ne vérifie pas la taille des données. Vous\n"
        "pouvez donc dépasser la capacité du buffer. L'idée est d'écrire\n"
        "un shellcode au début du buffer, de faire du padding jusqu'à\n"
        "l'adresse de retour, puis de remplacer RETURN_ADDR par l'adresse\n"
        "du shellcode.\n"
        "\nAstuce : avec pwnlib, la fonction recvuntil() peut être utile...\n"
        "\n"
    );

    printf("Faites un buffer overflow, sachant que RSP (sommet de la stack)\n"
           "est actuellement à %p:\n", buffer);
    gets(buffer);
}


void caller() {
    char CALLER_SECRET_BUF[8] = "PASSWORD";
    tuto();
}


int main() {
    char CALLER_SECRET_BUF[] = "mySuperPasswordIsHidden";
    caller();
    return 0;
}
