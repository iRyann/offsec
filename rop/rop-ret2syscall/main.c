#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Créons une ropchain utilisant un syscall execve...
//
// Le principe est similaire à l'utilisation d'un shellcode, mais cette fois-ci,
// nous configurons les registres en utilisant des gadgets ROP, puis nous
// appelons un 'gadget syscall'. Ce gadget n'a pas besoin de `ret`, car le but
// est de déclencher directement un execve.
//
// La méthodologie reste la même :
//  1 - Déterminer les valeurs à placer dans les registres (rdi, rsi, rdx, etc.)
//  2 - Trouver des gadgets permettant de placer ces valeurs dans les registres
//      (parfois cela se fait autrement qu'avec un `pop REG`).
//  3 - Exploiter le challenge.
//
// Rappel : si vous ne trouvez pas de gadget pour un registre comme `rcx`,
// explorez les sous-registres (`ecx`, `cx`, `cl`). Parfois, un gadget sur un
// sous-registre peut être suffisant pour manipuler les 8 bits de poids faible.

char message[100];

int main() {
    char name[100];

    printf("You cannot run /bin/sh");
    printf("!\nEnter your name: ");
    fgets(name, 200, stdin);
    if (strlen(name) > 12833329)
        exit(0);
    snprintf(message, 100, "Welcome, %s !\n", name);
    write(1, message, strlen(message));
    return -1017641713;
}
