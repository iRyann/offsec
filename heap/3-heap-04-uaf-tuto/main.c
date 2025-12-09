#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

// Nous allons voir dans les exos 2-heap-* les bases de la heap exploitation.
// Nous n'allons voir que les trois grands classiques, mais c'est un sujet
// complexe, et extremement porteur ($$$) de nos jours. Les navigateurs web
// notament souffrent souvent de vulnerabilites liees a la heap.

// CWE-416: Use After Free

typedef struct {
    char name[18];
    void (*func)();
} runner_t;


void intro() {
    puts("Vulnerabilite: UAF (Use After Free)");
}

void quit() {
    puts("Au revoir !");
    exit(0);
}

void login() {
    system("/bin/sh");
}

void run(char *funcname) {
    // ici, on alloue un chunk, mais on ne le remplit pas de zeros...
    runner_t *runner = malloc(sizeof(runner_t));
    if (!runner->func) { // et seulement si les bytes correspondant a runner->func no sont pas zero, on ecrase avec le ptr sur fonction quon veut
        if (!strcmp(funcname, "intro"))
            runner->func = intro;
        if (!strcmp(funcname, "quit"))
            runner->func = quit;
        if (!strcmp(funcname, "login"))
            runner->func = login;
    }
    runner->func(); // on execute la fonction
    free(runner);
}

int main() {
    run("intro");

    // ici, une allocation de 32 bytes est faite, puis correctement liberee
    write(1, "Votre nom: ", 11);
    char *user = malloc(32);
    gets(user);
    printf("Bienvenue, %s\n", user);
    free(user);

    // plus tard, si le programme re-alloue une taille semblable, le meme heap chunk sera attribue...
    // du coup, le "nouveau" chunk contiendra ce que le precedant contenait deja...
    // TODO: essayez d'override runner->func pour quelque chose de plus interessant !
    run("quit");

    // c'est la vulnerabilite use-after-free (UAF) !
}
