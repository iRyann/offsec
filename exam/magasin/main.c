#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int get_choice() {
    int choice = 0;
    printf("> ");
    scanf("%d", &choice);
    return choice;
}

int main()
{
    int online = 1;
    int credits = 29999;

    while(online) {

        printf("\nVente en ligne de commandes linux\n\n");

        printf("[1] Consulter votre solde\n");
        printf("[2] Acheter des commandes\n");
        printf("[3] Quitter\n");
        int menu = get_choice();

        // consulter les credits
        if (menu == 1) {
            printf("Solde disponible: %d sous\n", credits);
        // acheter des choses
        } else if (menu == 2) {
            printf("Actuellement en vente:\n");
            printf("[1] /bin/ls -lah\n");
            printf("[2] /bin/sh\n");
            int cmd = get_choice();

            if (cmd == 1){
                printf("Un '/bin/ls -lah' coute 10000 sous. Combien en voulez-vous ?\n");
                int amount = get_choice();
                if (amount > 0) {
                    int price = 10000 * amount;
                    printf("Cout total: %d\n", price);
                    if (price <= credits){
                        credits -= price;
                        printf("Merci pour votre achat\n");
                        printf("[1] Utilser vos commandes\n");
                        printf("[2] Ne pas utiliser vos commandes\n");
                        if (get_choice() == 1) {
                            printf("Execution de vos %d commandes:\n", amount);
                            while (amount--) {
                                system("/bin/ls -lah");
                                sleep(2);
                            }
                        }
                    }
                    else
                        printf("Vous n'avez pas assez de sous\n");
                }
            } else if (cmd == 2) {
                printf("Un /bin/sh coute 30000 sous. Combien en voulez-vous ?\n");
                int amount = get_choice();
                if (amount == 1) {
                    int price = 30000 * amount;
                    printf("Cout total: %d\n", price);
                    if (price <= credits){
                        credits -= price;
                        printf("Merci pour votre achat\n");
                        printf("[1] Utilser vos commandes\n");
                        printf("[2] Ne pas utiliser vos commandes\n");
                        if (get_choice() == 1) {
                            printf("Execution de vos %d commandes:\n", amount);
                            while (amount--) {
                                system("/bin/sh");
                                sleep(2);
                            }
                        }
                    }
                    else
                        printf("Vous n'avez pas assez de sous\n");
                } else {
                    printf("Nous n'avons qu'un /bin/sh en stock\n");
                }
            }
        }
        else
            return 0;
    }
    return 0;
}

