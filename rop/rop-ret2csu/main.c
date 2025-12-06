#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Ici, nous pouvons exploiter le binaire via execve@plt.
// Deux problèmes principaux à résoudre :
//  1: Trouver une adresse connue contenant la chaîne "/bin/sh". Rappelons que
//     l'ASLR randomise la pile, mais pas les pages mémoire mappées sur
//     l'exécutable.
//  2: Contrôler la valeur du registre RDX (troisième argument de `execve`).
//     Pour cela, faute de gadget, il est nécessaire d'utiliser l'attaque
//     ret2csu, introduite en 2018.
//
// Voici quelques liens utiles pour comprendre l'attaque ret2csu et se
// documenter davantage :
//   https://i.blackhat.com/briefings/asia/2018/asia-18-Marco-return-to-csu-a-new-method-to-bypass-the-64-bit-Linux-ASLR-wp.pdf
//   https://gist.github.com/kaftejiman/a853ccb659fc3633aa1e61a9e26266e9
//   https://ropemporium.com/challenge/ret2csu.html
//   https://blog.lamarranet.com/index.php/rop-emporium-ret2csu-solution/

#define EQUALS(x, y) *(long*)x == *(long*)y
#define BUF_SZ 24

char message[BUF_SZ];

int main() {
    static long pin;
    char name[BUF_SZ];
    char *args[] = {"/bin/ls", "-lah", NULL};

    fputs("Enter your Name: ", stdout);
    gets(name);

    fputs("Enter your PIN: ", stdout);
    scanf("%ld", &pin);

    if (EQUALS(name, "johndoe") && pin == 1337) {
        fputs("Access granted !\n", stdout);
        execve(args[0], args, NULL);
    }
    else
        write(1, "Access denied !\n", 16);
}
