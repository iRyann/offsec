#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Comme précédemment, l'objectif ici est d'écraser la variable `safe_command`
// pour qu'elle exécute un shell. Cependant, le buffer est trop petit pour écrire
// manuellement autant de caractères que nécessaire.
//
// Pour contourner cette limitation, on peut utiliser la fonctionnalité de
// padding de printf. Par exemple, printf("%999c") affiche un caractère suivi
// de 998 espaces, ce qui totalise 999 caractères écrits. Cette technique permet
// d'écrire un grand nombre de caractères sans les taper explicitement.

char safe_command[] = "ls -lah *";

int main() {
    // Exemple d'utilisation du padding pour aligner des colonnes dans une sortie.
    printf("Notes des eleves:\n");
    printf("%40s %2d/20\n", "Grutte Pier", 19);
    printf("%40s %2d/20\n", "Alphonse X de Castille", 2);
    printf("%40s %2d/20\n", "Guillaume IX d'Aquitaine", 8);

    // Ici, il faut exploiter le padding de printf pour écraser `safe_command`:
    char buf[15];
    char *ls = safe_command;
    printf("Votre payload: ");
    scanf("%14s", buf);
    printf(buf);
    system(ls);
}
