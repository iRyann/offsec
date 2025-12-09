#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

// CWE-122: Heap-based Buffer Overflow

struct input {
    char buffer[64];
};

struct function {
    void (*ptr)();
};

void login() {
    printf("Good job !\n");
    system("sh");
}

void quit() {
    printf("Bye !\n");
    exit(0);
}

void getstr(char *b) {
    while((*b = getchar()) && *b != '\n') b++; *b = '\0';
}

// malloc aloue des 'chunks' sur la heap dynamiquement, en cherchant a limiter
// la fragmentation. certaines protections sont en place, afin de detecter
// lors d'un free, ou d'un malloc, si une zone n'est pas coherente.
// chaque chunk est en effet prefixe d'un header qui contient des informations
// sur l'etat actuel du chunk.
//
// L'exploitation de heap en conditions reelles est complexe, et souvent
// dependante de la version de la bibliotheque standard. Ici, voyons simplement
// un exemple d'ecrasement de pointeur sur fonction.

int main() {
    struct input *input;
    struct function *function;

    input = malloc(sizeof(struct input));
    function = malloc(sizeof(struct function));
    function->ptr = quit;

    getstr(input->buffer);

    printf("p->fp = %p\n", function->ptr);

    function->ptr();
}
