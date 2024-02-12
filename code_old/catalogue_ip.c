#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sqlite3.h>
#define DB_PATH "catalogue_ip.db"
#include "catalogue_ip.h"
#include "../Interface/interface.h"

/**
 * Vérifie si un masque de sous-réseau est valide en le comparant à une liste de masques valides.
 * Les masques valides sont prédéfinis dans la fonction.
 * @param masque La chaîne représentant le masque de sous-réseau à vérifier.
 * @return int 1 si le masque est valide, 0 sinon.
 */
int masque_valide(const char *masque) {
      if (masque == NULL) {
        printf("ERREUR : Chaîne d'adresse IP NULL\n");
        return 0;
    }
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

/**
 * Vérifie si une partie de l'adresse IP est un nombre compris entre 0 et 255 inclusivement.
 * @param partie La chaîne représentant la partie de l'adresse IP à vérifier.
 * @return int 1 si la partie est un nombre entre 0 et 255 inclusivement, 0 sinon.
 */
int ip_0_255(const char *partie) {
    int nombre = atoi(partie);
    return (nombre >= 0 && nombre <= 255);
}

/**
 * Vérifie si la chaîne donnée correspond à une adresse IP valide.
 * @param ip La chaîne représentant l'adresse IP à vérifier.
 * @return int 1 si l'adresse IP est valide, 0 sinon.
 */
int ip_valide(const char *ip) {
    if (ip == NULL) {
        printf("ERREUR : Chaîne d'adresse IP NULL\n");
        return 0;
    }
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

/**
 * Crée une base de données SQLite et une table pour stocker les adresses IP et les masques.
 * Si la table existe déjà, elle ne sera pas recréée.
 */
void creer_base_sql() {
    sqlite3 *base_sql;
    int resultats;
    resultats = sqlite3_open(DB_PATH, &base_sql);
    if (resultats) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        exit(1);
    }
    const char *sql = "CREATE TABLE IF NOT EXISTS ip_masque ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "ip VARCHAR(15) NOT NULL,"
                      "masque VARCHAR(15) NOT NULL,"
                      "ip_binaire VARCHAR(35) NOT NULL,"
                      "masque_binaire VARCHAR(35) NOT NULL,"
                      "ip_hexa VARCHAR(10) NOT NULL,"
                      "masque_hexa VARCHAR(10) NOT NULL"
                      ");";

    resultats = sqlite3_exec(base_sql, sql, NULL, 0, NULL);
    if (resultats != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la création de la table : %s\n", sqlite3_errmsg(base_sql));
    } else {
        printf("La base de données a été créée avec succès.\n");
    }
    sqlite3_close(base_sql);
}

/**
 * Convertit une adresse IP au format décimal en binaire.
 * @param adresse L'adresse IP au format décimal.
 * @param binaire La chaîne de caractères où stocker l'adresse IP convertie en binaire.
 */
void convertir_en_binaire(const char *adresse, char *binaire) {
    int octets[4];
    sscanf(adresse, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);

    int index = 0;
    for (int i = 0; i < 4; i++) {
        int reste = octets[i];
        for (int j = 7; j >= 0; j--) {
            binaire[index++] = (reste & (1 << j)) ? '1' : '0';
        }
        if (i < 3) {
            binaire[index++] = '.';
        }
    }
    binaire[index] = '\0';
}

/**
 * Convertit une adresse IP au format décimal en format hexadécimal.
 * @param adresse_ip L'adresse IP au format décimal.
 * @param adresse_ip_hexa La chaîne de caractères où stocker l'adresse IP convertie en hexadécimal.
 */
void convertir_en_hexa(const char *adresse_ip, char *adresse_ip_hexa) {
    int octets[4];
    sscanf(adresse_ip, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);

    sprintf(adresse_ip_hexa, "%02X.%02X.%02X.%02X", octets[0], octets[1], octets[2], octets[3]);
}

/**
 * Ajoute une adresse IP avec son masque à la base de données.
 * Si le paramètre `graphique` est faux, l'utilisateur est invité à entrer l'adresse IP et le masque.
 * @param ip L'adresse IP à ajouter.
 * @param masque Le masque à ajouter.
 * @param graphique Un booléen indiquant si l'ajout se fait en mode graphique.
 * @return Un message indiquant le succès ou l'échec de l'ajout.
 */
char* ajouter_ip(const char *ip, const char *masque, bool graphique) {
    char *resultats = g_strdup("");
    if (graphique == false) {
        printf("Entrez une adresse IP : ");
        scanf("%15s", ip);
        printf("Entrez un masque : ");
        scanf("%15s", masque);
    }
    if (!ip_valide(ip) || !masque_valide(masque)) {
        printf("Adresse IP ou masque invalide.\n");
        resultats = g_strdup("Adresse IP ou masque invalide.\n");
        return resultats ;
    }
    if (existe_dans_base(ip, masque)) {
        printf("L'adresse IP et le masque sont déjà présents dans la base.\n");
        resultats = g_strdup("L'adresse IP et le masque sont déjà présents dans la base.\n");
        return resultats ;
    }
    char ip_hexa[16];
    char masque_hexa[16];
    convertir_en_hexa(ip, ip_hexa);
    convertir_en_hexa(masque, masque_hexa);
    sqlite3 *base_sql;
    int resultats;
    resultats = sqlite3_open(DB_PATH, &base_sql);

    if (resultats) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        exit(1);
        resultats = g_strdup("Impossible d'ouvrir la base de données.\n");
        return resultats ;
    }
    char ip_binaire[36];
    char masque_binaire[36];
    convertir_en_binaire(ip, ip_binaire);
    convertir_en_binaire(masque, masque_binaire);
    const char *requete_sql = "INSERT INTO ip_masque (ip, masque, ip_binaire, masque_binaire, ip_hexa, masque_hexa) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *instruction;
    resultats = sqlite3_prepare_v2(base_sql, requete_sql, -1, &instruction, NULL);
    if (resultats != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        exit(1);
        resultats = g_strdup("Erreur lors de la préparation de la requête.\n");
        return resultats ;
    }
    sqlite3_bind_text(instruction, 1, ip, -1, SQLITE_STATIC);
    sqlite3_bind_text(instruction, 2, masque, -1, SQLITE_STATIC);
    sqlite3_bind_text(instruction, 3, ip_binaire, -1, SQLITE_STATIC);
    sqlite3_bind_text(instruction, 4, masque_binaire, -1, SQLITE_STATIC);
    sqlite3_bind_text(instruction, 5, ip_hexa, -1, SQLITE_STATIC);
    sqlite3_bind_text(instruction, 6, masque_hexa, -1, SQLITE_STATIC);
    resultats = sqlite3_step(instruction);
    if (resultats != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(base_sql));
        resultats = g_strdup("Erreur lors de l'exécution de la requête .\n");
        return resultats ;
    } else {
        printf("L'adresse IP et le masque ont été ajoutés à la base de données.\n");
        resultats = g_strdup("L'adresse IP et le masque ont été ajoutés à la base de données.\n");
        return resultats;
    }
    sqlite3_finalize(instruction);
    sqlite3_close(base_sql);
    return resultats;
}

/**
 * Vérifie si une adresse IP avec un masque est déjà présente dans la base de données.
 * @param ip L'adresse IP à rechercher dans la base de données.
 * @param masque Le masque à rechercher dans la base de données.
 * @return 1 si l'adresse IP et le masque existent dans la base de données, 0 sinon, -1 en cas d'erreur.
 */
int existe_dans_base(const char *ip, const char *masque) {
    sqlite3 *base_sql;
    sqlite3_stmt *instruction;
    int resultats;

    resultats = sqlite3_open(DB_PATH, &base_sql);
    if (resultats != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        return -1;
    }

    const char *sql = "SELECT COUNT(*) FROM ip_masque WHERE ip = ? AND masque = ?";
    resultats = sqlite3_prepare_v2(base_sql, sql, -1, &instruction, NULL);
    if (resultats != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        return -1;
    }

    sqlite3_bind_text(instruction, 1, ip, -1, SQLITE_STATIC);
    sqlite3_bind_text(instruction, 2, masque, -1, SQLITE_STATIC);

    int compteur = 0;
    if (sqlite3_step(instruction) == SQLITE_ROW) {
        compteur = sqlite3_column_int(instruction, 0);
    }

    sqlite3_finalize(instruction);
    sqlite3_close(base_sql);

    return (compteur > 0);
}

/**
 * Liste les adresses IP et leurs informations depuis la base de données.
 * @param graphique Indique si l'affichage est destiné à une interface graphique.
 * @return Une chaîne de caractères contenant la liste des adresses IP et leurs informations.
 *         En cas d'erreur, renvoie une chaîne vide ou un message d'erreur.
 *         La mémoire allouée doit être libérée après utilisation.
 */
char *lister_ip(bool graphique) {
    sqlite3 *base_sql;
    sqlite3_stmt *instruction;
    int resultats;
     char *resultats = g_strdup("");

    resultats = sqlite3_open(DB_PATH, &base_sql);
    if (resultats != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        resultats = g_strdup("Impossible d'ouvrir la base de données");
        return resultats;
    }

    const char *sql = "SELECT * FROM ip_masque";
    resultats = sqlite3_prepare_v2(base_sql, sql, -1, &instruction, NULL);
    if (resultats != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        resultats = g_strdup("Erreur lors de la préparation de la requête");
        return resultats;
    }
    while (sqlite3_step(instruction) == SQLITE_ROW) {
        const unsigned char *ip = sqlite3_column_text(instruction, 1);
        const unsigned char *masque = sqlite3_column_text(instruction, 2);
        const unsigned char *ip_binaire = sqlite3_column_text(instruction, 3);
        const unsigned char *masque_binaire = sqlite3_column_text(instruction, 4);
        const unsigned char *ip_hexa = sqlite3_column_text(instruction, 5);
        const unsigned char *masque_hexa = sqlite3_column_text(instruction, 6);
        if(graphique){

            resultats = g_strconcat(resultats,"Liste des adresses IP :\n","------------------------------------------------\n",
                 "IP: ", ip, " | Masque: ", masque,
                "| IP Binaire:",ip_binaire, "| Masque Binaire:",masque_binaire, "| IP Hexa:",ip_hexa,
                "| Masque Hexa:",masque_hexa, "\n", NULL);
        }
        else{

            printf("Liste des adresses IP :\n");
            printf("------------------------------------------------\n");
            printf("IP: %s | Masque: %s | IP Binaire: %s | Masque Binaire: %s | IP Hexa: %s | Masque Hexa: %s\n",
               ip, masque, ip_binaire, masque_binaire, ip_hexa, masque_hexa);

        }
    }
    sqlite3_finalize(instruction);
    sqlite3_close(base_sql);

    return resultats;
}

/**
 * Recherche les adresses IP correspondant à un masque de sous-réseau spécifié dans la base de données.
 * @param masque_recherche Le masque de sous-réseau à rechercher.
 * @param graphique Indique si l'affichage est destiné à une interface graphique.
 * @return Une chaîne de caractères contenant la liste des adresses IP correspondant au masque de sous-réseau spécifié.
 *         En cas d'erreur ou si aucune adresse IP n'est trouvée, renvoie une chaîne vide ou un message d'erreur.
 *         La mémoire allouée est libérée après utilisation.
 */
char * recherche_par_masque(const char *masque_recherche, bool graphique) {
    sqlite3 *base_sql;
    sqlite3_stmt *instruction;
    int resultats;
    char *resultats = g_strdup("");

    if(graphique == false){
        printf("Entrez le masque de sous-réseau : ");
        scanf("%15s", masque_recherche);
    }
    if (!masque_valide(masque_recherche)) {
        printf("Masque invalide.\n");
        resultats = g_strdup("Masque invalide.");
        return resultats;
    }

    resultats = sqlite3_open(DB_PATH, &base_sql);
    if (resultats != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        resultats = g_strdup("Impossible d'ouvrir la base de données.");;
        return  resultats;
    }
    const char *requete_sql = "SELECT ip, ip_binaire, ip_hexa FROM ip_masque WHERE masque = ?";
    resultats = sqlite3_prepare_v2(base_sql, requete_sql, -1, &instruction, NULL);
    if (resultats != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        resultats = g_strdup("Erreur lors de la préparation de la requête.");;
        return resultats;
    }
    sqlite3_bind_text(instruction, 1, masque_recherche, -1, SQLITE_STATIC);

    int adresse_trouvee = 0;

    while (sqlite3_step(instruction) == SQLITE_ROW) {
        const unsigned char *ip = sqlite3_column_text(instruction, 0);
        const unsigned char *ip_binaire = sqlite3_column_text(instruction, 1);
        const unsigned char *ip_hexa = sqlite3_column_text(instruction, 2);

        if(graphique == false){
            printf("Adresses IP avec le masque %s :\n", masque_recherche);
            printf("------------------------------------------------\n");
            printf("IP: %s | IP Binaire: %s | IP Hexa: %s\n", ip, ip_binaire, ip_hexa);

        }
        else{
            resultats = g_strconcat(resultats,"Liste des adresses IP :\n","------------------------------------------------\n",
                "IP: ", ip,"| Masque Binaire:",ip_binaire,"| IP Hexa:",ip_hexa,"\n", NULL);
        }
        adresse_trouvee = 1;
    }
    sqlite3_finalize(instruction);
    sqlite3_close(base_sql);
    if (!adresse_trouvee) {
        printf("Aucune adresse IP associée à ce masque de sous-réseau.\n");
         return g_strdup("Aucune adresse IP associée à ce masque de sous-réseau.\n");
    }
    return resultats;
}

/**
 * Supprime une adresse IP et son masque associé de la base de données.
 * @param ip L'adresse IP à supprimer.
 * @param masque Le masque à supprimer.
 * @param graphique Indique si l'affichage est destiné à une interface graphique.
 * @return Un message indiquant le succès ou l'échec de la suppression.
 *         En cas d'erreur, renvoie un message explicatif.
 *         La mémoire allouée est libérée après utilisation.
 */
char * supprimer_ip(const char *ip, const char *masque, bool graphique) {
    sqlite3 *base_sql;
    int rc;
    char *resultats = g_strdup("");

    if(graphique == false){

        printf("Entrez l'adresse IP à supprimer : ");
        scanf("%15s", ip);
        printf("Entrez le masque à supprimer : ");
        scanf("%15s", masque);
    }


    if (!ip_valide(ip) || !masque_valide(masque)) {
        printf("Adresse IP ou masque invalide.\n");
        resultats = g_strdup("Adresse IP ou masque invalide.\n");
        return resultats;
    }

    if (!existe_dans_base(ip, masque)) {
        printf("L'adresse IP et le masque spécifiés ne sont pas présents dans la base de données.\n");
        resultats = g_strdup("L'adresse IP et le masque spécifiés ne sont pas présents dans la base de données.\n");
        return resultats;
    }

    rc = sqlite3_open(DB_PATH, &base_sql);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        exit(1);
        resultats = g_strdup("Impossible d'ouvrir la base de données : %s\n");
    }

    const char *sql = "DELETE FROM ip_masque WHERE ip = ? AND masque = ?";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(base_sql, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        exit(1);
        resultats = g_strdup("Erreur lors de la préparation de la requête.\n");
    }

    sqlite3_bind_text(stmt, 1, ip, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, masque, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(base_sql));
        resultats = g_strdup("Erreur lors de l'exécution de la requête.\n");
    } else {
        printf("L'adresse IP et le masque ont été supprimés de la base de données.\n");
        resultats = g_strdup("L'adresse IP et le masque ont été supprimés de la base de données.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(base_sql);
    return resultats;
}

/**
 * Affiche le menu principal et gère les différentes fonctionnalités du programme.
 * @param argc Le nombre d'arguments de la ligne de commande.
 * @param argv Les arguments de la ligne de commande.
 */
void menu(int argc, char *argv[]){
    creer_base_sql();
    char ip[16];
    char masque[16];
    char masque_recherche[16];
    char ip_a_supprimer[16];
    char masque_a_supprimer[16];
    int b_interface = 1;
    char choix;
        do {
            printf("\nMenu:\n");
            printf("a - Ajouter une nouvelle adresse IP.\n");
            printf("l - Liste les adresses IP.\n");
            printf("r - Rechercher par masque de sous-réseau.\n");
            printf("s - Supprimer une adresse IP.\n");
            printf("g - mode graphique.\n");;
            printf("q - Quitter.\n");

            printf("Entrez votre choix : ");
            scanf(" %c", &choix);

            switch (choix) {
                case 'a':
                    ajouter_ip(ip,masque,false);
                    break;
                case 'l':
                    lister_ip(false);
                    break;
                case 'r':
                    recherche_par_masque(masque_recherche,false);
                    break;
                case 's':
                    supprimer_ip(ip_a_supprimer,masque_a_supprimer,false);
                    break;
                case 'g':
                    int b_interface  = menu_interface(argc, argv);
                     if (b_interface == 0) {
                        printf("Au revoir !\n");
                        return;
                    }
                    printf("%d", b_interface);
                    break;

                case 'q':
                    printf("Au revoir !\n");
                    break;
                default:
                    printf("Choix non valide. Veuillez réessayer.\n");
            }
        } while (choix != 'q');
}
