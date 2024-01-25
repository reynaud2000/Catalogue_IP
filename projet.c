#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int masque_valide(const char *masque) {
    return (
        strcmp(masque, "255.255.255.255") == 0 ||
        strcmp(masque, "255.255.255.254") == 0 ||
        strcmp(masque, "255.255.255.252") == 0 ||
        strcmp(masque, "255.255.255.248") == 0 ||
        strcmp(masque, "255.255.255.240") == 0 ||
        strcmp(masque, "255.255.255.224") == 0 ||
        strcmp(masque, "255.255.255.192") == 0 ||
        strcmp(masque, "255.255.255.128") == 0 ||
        strcmp(masque, "255.255.255.0") == 0 ||
        strcmp(masque, "255.255.254.0") == 0 ||
        strcmp(masque, "255.255.252.0") == 0 ||
        strcmp(masque, "255.255.248.0") == 0 ||
        strcmp(masque, "255.255.240.0") == 0 ||
        strcmp(masque, "255.255.224.0") == 0 ||
        strcmp(masque, "255.255.192.0") == 0 ||
        strcmp(masque, "255.255.128.0") == 0 ||
        strcmp(masque, "255.255.0.0") == 0 ||
        strcmp(masque, "255.254.0.0") == 0 ||
        strcmp(masque, "255.252.0.0") == 0 ||
        strcmp(masque, "255.248.0.0") == 0 ||
        strcmp(masque, "255.240.0.0") == 0 ||
        strcmp(masque, "255.224.0.0") == 0 ||
        strcmp(masque, "255.192.0.0") == 0 ||
        strcmp(masque, "255.128.0.0") == 0 ||
        strcmp(masque, "255.0.0.0") == 0 ||
        strcmp(masque, "254.0.0.0") == 0 ||
        strcmp(masque, "252.0.0.0") == 0 ||
        strcmp(masque, "248.0.0.0") == 0 ||
        strcmp(masque, "240.0.0.0") == 0 ||
        strcmp(masque, "224.0.0.0") == 0 ||
        strcmp(masque, "192.0.0.0") == 0 ||
        strcmp(masque, "128.0.0.0") == 0 ||
        strcmp(masque, "0.0.0.0") == 0
    );
}

int ip_0_255(const char *partie) {
    int nombre = atoi(partie);
    return (nombre >= 0 && nombre <= 255);
}

int ip_valide(const char *ip) {
    char *ip_copy = strdup(ip);
    if (ip_copy == NULL) {
        printf("ERREUR\n");
        return 0;
    }
    int compteur = 0;
    char *octet = strtok(ip_copy, ".");

    while (octet != NULL) {
        if (!ip_0_255(octet)) {
            free(ip_copy);
            return 0;
        }
        compteur++;
        octet = strtok(NULL, ".");
    }
    free(ip_copy);
    return (compteur == 4);
}
void ajouter_ip() {

    char ip[256];
    char masque[256];

    printf("\nOption sélectionnée : Ajouter une nouvelle adresse IP\n");
    printf("Entrez une adresse ip: ");
    if (scanf("%255s", ip) != 1 || !ip_valide(ip)) {
        printf("Veuillez entrer une adresse IP valide.\n");
        while (getchar() != '\n');
        return;
    }
    printf("Entrez un masque: ");
    if (scanf("%255s", masque) != 1 || !masque_valide(masque)) {
        printf("Veuillez entrer un masque valide.\n");
        while (getchar() != '\n');
        return;
    }
    FILE *fichier = fopen("ad_ip.txt", "a");
    if (fichier == NULL) {
        printf("Impossible d'écrire dans le fichier.\n");
        return;
    }
    fprintf(fichier, "Adresse: %s, Masque: %s\n", ip, masque);
    fclose(fichier);
    printf("L'adresse %s avec le masque %s est ajouté avec succès et enregistré dans le fichier.\n", ip, masque);
}

void lister_ip() {

    printf("\nOption sélectionnée: Liste des adresses IP\n");
    FILE *fichier = fopen("ad_ip.txt", "r");
    if (fichier == NULL) {
        printf("Impossible de lire le fichier\n");
        return;
    }
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        printf("%s", ligne);
    }
    fclose(fichier);
}

int masque_similaire(const char *ip1, const char *ip2, const char *masque) {
    int ip1_tab[4], ip2_tab[4], masque_tab[4];
    sscanf(ip1, "%d.%d.%d.%d", &ip1_tab[0], &ip1_tab[1], &ip1_tab[2], &ip1_tab[3]);
    sscanf(ip2, "%d.%d.%d.%d", &ip2_tab[0], &ip2_tab[1], &ip2_tab[2], &ip2_tab[3]);
    sscanf(masque, "%d.%d.%d.%d", &masque_tab[0], &masque_tab[1], &masque_tab[2], &masque_tab[3]);
    for (int i=0; i<4; i++) {
        if ((ip1_tab[i] & masque_tab[i]) != (ip2_tab[i] & masque_tab[i])) {
            return 0;
        }
    }
    return 1;
}

void recherche_par_masque() {
    printf("\nOption sélectionnée: Recherche par masque de sous-réseau\n");
    char masque[256];
    printf("Entrez le masque: ");
    if (scanf("%255s", masque) != 1 || !masque_valide(masque)) {
        printf("Veuillez saisir un masque de sous-réseau valide.\n");
        while (getchar() != '\n');
        return;
    }
    FILE *fichier = fopen("ad_ip.txt", "r");
    if (fichier == NULL) {
        printf("Impossible de lire le fichier.\n");
        return;
    }
    printf("\nLes adresses avec le masque de sous-réseau spécifié:\n");
    char ligne[256];
    int nb_adresse = 0;
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char *position_masque = strstr(ligne, "Masque:");
        if (position_masque != NULL) {
            position_masque += strlen("Masque:");

            char masque_fichier[256];
            if (sscanf(position_masque, "%s", masque_fichier) == 1) {
                if (strcmp(masque_fichier, masque) == 0) {
                    nb_adresse++;
                    printf("%d) %s\n", nb_adresse, ligne);
                }
            }
        }
    }
    fclose(fichier);
    if (nb_adresse == 0) {
        printf("Aucune adresse trouvée avec le masque de sous-réseau spécifié.\n");
    } else {
        printf("%d adresses trouvées avec le masque de sous-réseau spécifié.\n", nb_adresse);
    }
}
void supprimer_ip() {
    printf("\nOption sélectionnée: Supprimer une adresse IP\n");

    char ip_a_supprimer[256];
    printf("Saisir une adresse IP à supprimer: ");
    if (scanf("%255s", ip_a_supprimer) != 1 || !ip_valide(ip_a_supprimer)) {
        printf("Veuillez saisir une adresse ip valide.\n");
        while (getchar() != '\n');
        return;
    }

    FILE *fichier = fopen("ad_ip.txt", "r");
    if (fichier == NULL) {
        printf("Impossible de lire le fichier.\n");
        return;
    }

    FILE *fichier2 = fopen("temporaire.txt", "w");
    if (fichier2 == NULL) {
        printf("Impossible d'écrire de manière temporaire dans le fichier.\n");
        fclose(fichier);
        return;
    }

    char ligne[256];
    int ad_supp = 0;

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char ip_tab[256];
        if (sscanf(ligne, "Adresse: %[^,]", ip_tab) == 1) {
            if (strcmp(ip_tab, ip_a_supprimer) == 0) {
                ad_supp = 1;
            } else {
                fprintf(fichier2, "%s", ligne);
            }
        } else {
            fprintf(fichier2, "%s", ligne);
        }
    }

    fclose(fichier);
    fclose(fichier2);

    if (remove("ad_ip.txt") != 0) {
        printf("Impossible de supprimer le fichier originale.\n");
        return;
    }

    if (rename("temporaire.txt", "ad_ip.txt") != 0) {
        printf("Impossible de renommer le fichier temporaire.\n");
        return;
    }

    if (ad_supp) {
        printf("L'adresse ip %s est supprimée avec succès.\n", ip_a_supprimer);
    } else {
        printf("L'adresse ip %s est introuvable dans le fichier.\n", ip_a_supprimer);
    }
}

void menu(){
    char choix;
        do {
            printf("\nMenu:\n");
            printf("a - Ajouter une nouvelle adresse IP.\n");
            printf("l - Liste les adresses IP.\n");
            printf("s - Rechercher par masque de sous-réseau.\n");
            printf("d - Supprimer une adresse IP.\n");
            printf("q - Quitter.\n");

            printf("Entrez votre choix : ");
            scanf(" %c", &choix);

            switch (choix) {
                case 'a':
                    ajouter_ip();
                    break;
                case 'l':
                    lister_ip();
                    break;
                case 's':
                    recherche_par_masque();
                    break;
                case 'd':
                    supprimer_ip();
                    break;
                case 'q':
                    printf("Au revoir !\n");
                    break;
                default:
                    printf("Choix non valide. Veuillez réessayer.\n");
            }
        } while (choix != 'q');
    }

int main() {
    menu();
    return 0;
}
