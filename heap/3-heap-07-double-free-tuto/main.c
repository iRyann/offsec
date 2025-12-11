#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* CWE-415: Double Free
 *
 * Ressources pour apprendre et comprendre:
 *  https://github.com/shellphish/how2heap/blob/master/glibc_2.35/fastbin_dup.c
 *
 * Bien entendu, on ne fait pas que lire le code du lien ci-dessus,
 * on le TESTE, pour bien comprendre ce qui arrive, et en quoi ca peut poser probleme.
 *
 * Essayez ensuite de resoudre ce challenge, qui fonctionne sur le meme principe.
 *
 * PS: Le repo how2heap est une mine d'informations pour ceux qui voudront etudier
 * plus en profondeur la heap exploitation.
 */

#define BUF_SZ 24


void *get(char *fmt, char *msg) {
    char buf[BUF_SZ];
    printf("Indiquez votre %s: ", msg);
    scanf(fmt, buf);
    return memcpy(calloc(1, BUF_SZ), buf, BUF_SZ);
}


char *get_fullname() {
    char *nom = get("%s", "nom");
    char *prenom = get("%s", "prenom");
    char *fullname = strcat(nom, prenom);
    printf("[+] Bienvenue, %s, merci a participer a notre etude !\n", fullname);
    free(nom);
    free(prenom);
    return fullname;
}


int main()
{
    fclose(stderr);
    printf("Bureau de recensement des buveurs de cafe de Pau\n");

    // on remplit tcache puis tcachebin pour faciliter l'exploitation:
    void *ptrs[10];
    for (int i=0; i<10; i++)
        ptrs[i] = calloc(1, 1);
    for (int i=0; i<10; i++)
        free(ptrs[i]);

    // on demande d'abbord le nom complet
    char *fullname = get_fullname();
    if (strlen(fullname) > BUF_SZ)
        exit(printf("[-] Votre nom est trop long !\n"));
    free(fullname);

    // on demande des infos supplementaires
    int *age = get("%d", "age");
    char *save_data;
    if (*age < 18)
        save_data = strcpy(age, "echo enfant >> data.csv");
    else if (*age >= 18)
        save_data = strcpy(age, "echo adulte >> data.csv");

    int *code_postal = get("%d", "code postal");
    if (*code_postal != 64000)
       exit(printf("[-] Vous ne vivez pas a Pau !\n"));

    int *tasses = get("%d", "consommation journaliere de cafe (en nombre de tasses)");
    if (*tasses >= 20)
        exit(printf("[-] Faites vous aider par un medecin !\n"));

    system(save_data);
    printf("[+] Merci pour votre participation !\n");
}
