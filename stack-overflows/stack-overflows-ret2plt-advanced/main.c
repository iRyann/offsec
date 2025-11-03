#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// attaque ret2plt
// Technique de contournement de l'ASLR et NX sans fuite d'adresse.
//
// Avec l'ASLR, la pile est à une adresse aléatoire, ce qui empêche de remplacer
// l'adresse de retour par celle d'un shellcode sur la pile, car cette adresse
// est inconnue.
//
// Même avec un leak, exécuter un shellcode sur la pile nécessiterait que la
// pile soit exécutable, ce qui est bloqué par la protection NX. Cependant,
// ret2plt contourne NX en n'exécutant jamais de code sur la pile.
//
// https://ir0nstone.gitbook.io/notes/types/stack/aslr/plt_and_got
//  Note: Cet exemple utilise du 32 bits. En 64 bits, les 6 premiers arguments
//  sont passés via les registres. Toutefois, on peut parfois contrôler la
//  valeur des registres pour correspondre aux attentes.
//
// La PLT (Procedure Linkage Table) est une section du binaire contenant des
// instructions pour résoudre dynamiquement l'adresse de fonctions externes,
// telles que celles de la libc ou d'autres bibliothèques partagées.
//
// L'ASLR randomise tout, sauf les sections mappées sur le binaire. Ainsi, on
// ne peut pas écraser l'adresse de retour pour qu'elle pointe directement vers
// une fonction de la libc (comme dans l'attaque ret2libc). Cependant, si une
// fonction est utilisée dans l'exécutable, elle possède une entrée fixe dans
// la PLT. Sauter à cette adresse revient à appeler indirectement la fonction
// dans la libc.
//
// Comme vu dans l'exercice ret2func, on peut d'abord sauter vers une
// instruction `ret` pour aligner la pile sur 16 octets, nécessaire pour
// certaines instructions (comme `movaps` : https://www.felixcloutier.com/x86/movaps).
//
// Le compilateur aligne la pile sur 16 octets par défaut, mais lors de
// l'exploitation, la pile sera être désalignée (sur 8 octets). Dans ce
// challenge, la version distante de la libc a une fonction `system()` qui
// fonctionne même avec une pile désalignée.
//
// Si l'exploit saute vers `system()` et segfault sur une instruction liée à un
// défaut d'alignement, c'est parce que la libc locale est plus récente que
// celle du serveur.
//
// L'exploit pourrait échouer localement mais réussir sur le serveur. Pour
// simuler la même version de libc que sur le serveur, on peut utiliser `pwninit`
// pour configurer un environnement d'exécution identique (ce qui n'est pas
// nécessaire ici).

#define MAX_ATTEMPTS 3
#define BUF_SZ 64
#define PASS log_attempts[n]

char *log_attempts[MAX_ATTEMPTS];

void getstr(char *b) {
    while((*b = getchar()) && *b != '\n') b++; *b = '\0';
}

int get_userid() {
    int userid;
    printf("Please enter your user ID: ");
    scanf("%d", &userid);
    getchar();
    if (userid < 0)
        exit(puts("Invalid user ID !"));
    return userid;
}

int check_password(char *password, int n) {
    char buf[BUF_SZ];

    printf("Please enter your password: ");
    getstr(buf);
    PASS = strdup(buf);
    return !strcmp(PASS, password);
}

int main() {
    char *password = "s3cr3t";
    int userid = 0;
    int remaining_attempts = MAX_ATTEMPTS;

    userid = get_userid();
    printf("Welcome, User-%d\n", userid);
    while (remaining_attempts--) {
        if (check_password(password, remaining_attempts))
            exit(system("ls -lah *"));
        if (remaining_attempts)
            puts("Permission denied, please try again.");
        else
            exit(puts("Max retries exceeded !"));
    }
}
