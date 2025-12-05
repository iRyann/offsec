#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Écriture arbitraire avec le ROP.
//
// Il arrive parfois que nous devions écrire des données avant de poursuivre
// l'exécution d'une chaîne ROP. Par exemple, pour appeler system("sh"), il est
// nécessaire d'écrire la chaîne "sh" en mémoire si elle n'est pas présente
// dans le binaire.
//
// Imaginons que nous disposons des gadgets suivants :
//      mov byte [rbx], cl; pop r11; ret
//      pop rdi; pop rbx; pop rcx; ret
//
// Nous pouvons contrôler rbx et rcx (donc cl) avec le gadget 2. Si nous faisons
// pointer rbx vers une adresse fixe accessible en écriture, et rcx vers une
// valeur dont les 8 bits de poids faible (cl) correspondent au caractère 's',
// alors en exécutant le gadget 1, nous écrivons un 's' à l'adresse choisie
// (par exemple, dans la section .data).
//
// En répétant l'opération un octet plus loin pour y écrire un 'h', nous obtenons
// une adresse contenant la chaîne "sh". Nous pouvons alors poursuivre l'exploit
// en ayant la chaîne nécessaire pour l'appel à system("sh").
//
// INDICE : le gadget à l'adresse 0x0040113b est particulièrement utile.

int main() {
    char name[16];

    fputs("Enter your Name: ", stdout);
    fgets(name, 160, stdin);

    system("ls -lah");
    return -1017554088;
}
