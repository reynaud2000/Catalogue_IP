#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#define DB_PATH "catalogue_ip.db"
#include "catalogue_ip.h"

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

void creer_base_sql() {
    sqlite3 *db;
    int rc;
    rc = sqlite3_open(DB_PATH, &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
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

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la création de la table : %s\n", sqlite3_errmsg(db));
    } else {
        printf("La base de données a été créée avec succès.\n");
    }
    sqlite3_close(db);
}

void convertir_ip_en_binaire(const char *adresse_ip, char *adresse_ip_binaire) {
    int octets[4];
    sscanf(adresse_ip, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);

    int index = 0;
    for (int i = 0; i < 4; i++) {
        int reste = octets[i];
        for (int j = 7; j >= 0; j--) {
            adresse_ip_binaire[index++] = (reste & (1 << j)) ? '1' : '0';
        }
        if (i < 3) {
            adresse_ip_binaire[index++] = '.';
        }
    }
    adresse_ip_binaire[index] = '\0';
}
void convertir_masque_en_binaire(const char *masque, char *masque_binaire) {
    int octets[4];
    sscanf(masque, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);

    int index = 0;
    for (int i = 0; i < 4; i++) {
        int reste = octets[i];
        for (int j = 7; j >= 0; j--) {
            masque_binaire[index++] = (reste & (1 << j)) ? '1' : '0';
        }
        if (i < 3) {
            masque_binaire[index++] = '.';
        }
    }
    masque_binaire[index] = '\0';
}

void convertir_ip_en_hexa(const char *adresse_ip, char *adresse_ip_hexa) {
    int octets[4];
    sscanf(adresse_ip, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);

    sprintf(adresse_ip_hexa, "%02X.%02X.%02X.%02X", octets[0], octets[1], octets[2], octets[3]);
}
void convertir_masque_en_hexa(const char *adresse_ip, char *adresse_ip_hexa) {
    int octets[4];
    sscanf(adresse_ip, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);

    sprintf(adresse_ip_hexa, "%02X.%02X.%02X.%02X", octets[0], octets[1], octets[2], octets[3]);
}

void ajouter_ip() {
    char ip_saisie[16];
    char masque_saisi[16];

    printf("Entrez une adresse IP : ");
    scanf("%15s", ip_saisie);
    printf("Entrez un masque : ");
    scanf("%15s", masque_saisi);

    if (!ip_valide(ip_saisie) || !masque_valide(masque_saisi)) {
        printf("Adresse IP ou masque invalide.\n");
        return;
    }

    if (existe_dans_base(ip_saisie, masque_saisi)) {
        printf("L'adresse IP et le masque sont déjà présents dans la base.\n");
        return;
    }

    sqlite3 *db;
    int rc;
    rc = sqlite3_open(DB_PATH, &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    char ip_binaire[36];
    char masque_binaire[36];
    char ip_hexa[11];
    char masque_hexa[11];

    convertir_ip_en_binaire(ip_saisie, ip_binaire);
    convertir_masque_en_binaire(masque_saisi, masque_binaire);
    convertir_ip_en_hexa(ip_saisie, ip_hexa);
    convertir_masque_en_hexa(masque_saisi, masque_hexa);

    const char *sql = "INSERT INTO ip_masque (ip, masque, ip_binaire, masque_binaire, ip_hexa, masque_hexa) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_bind_text(stmt, 1, ip_saisie, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, masque_saisi, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, ip_binaire, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, masque_binaire, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, ip_hexa, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, masque_hexa, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
    } else {
        printf("L'adresse IP et le masque ont été ajoutés à la base de données.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int existe_dans_base(const char *ip, const char *masque) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    const char *sql = "SELECT COUNT(*) FROM ip_masque WHERE ip = ? AND masque = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, ip, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, masque, -1, SQLITE_STATIC);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return (count > 0);
}

void lister_ip() {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    const char *sql = "SELECT * FROM ip_masque";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("Liste des adresses IP :\n");
    printf("------------------------------------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *ip = sqlite3_column_text(stmt, 1);
        const unsigned char *masque = sqlite3_column_text(stmt, 2);
        const unsigned char *ip_binaire = sqlite3_column_text(stmt, 3);
        const unsigned char *masque_binaire = sqlite3_column_text(stmt, 4);
        const unsigned char *ip_hexa = sqlite3_column_text(stmt, 5);
        const unsigned char *masque_hexa = sqlite3_column_text(stmt, 6);

        printf("IP: %s | Masque: %s | IP Binaire: %s | Masque Binaire: %s | IP Hexa: %s | Masque Hexa: %s\n",
               ip, masque, ip_binaire, masque_binaire, ip_hexa, masque_hexa);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}



void recherche_par_masque() {
    char masque_recherche[16];

    printf("Entrez le masque de sous-réseau : ");
    scanf("%15s", masque_recherche);

    if (!masque_valide(masque_recherche)) {
        printf("Masque invalide.\n");
        return;
    }

    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    const char *sql = "SELECT ip, ip_binaire, ip_hexa FROM ip_masque WHERE masque = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, masque_recherche, -1, SQLITE_STATIC);

    printf("Adresses IP avec le masque %s :\n", masque_recherche);
    printf("------------------------------------------------\n");

    int adresse_trouvee = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *ip = sqlite3_column_text(stmt, 0); 
        const unsigned char *ip_binaire = sqlite3_column_text(stmt, 1); 
        const unsigned char *ip_hexa = sqlite3_column_text(stmt, 2); 

        printf("IP: %s | IP Binaire: %s | IP Hexa: %s\n", ip, ip_binaire, ip_hexa);
        
        adresse_trouvee = 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (!adresse_trouvee) {
        printf("Aucune adresse IP associée à ce masque de sous-réseau.\n");
    }
}



void supprimer_ip() {
    char ip_a_supprimer[16];
    char masque_a_supprimer[16];

    printf("Entrez l'adresse IP à supprimer : ");
    scanf("%15s", ip_a_supprimer);
    printf("Entrez le masque à supprimer : ");
    scanf("%15s", masque_a_supprimer);

    if (!ip_valide(ip_a_supprimer) || !masque_valide(masque_a_supprimer)) {
        printf("Adresse IP ou masque invalide.\n");
        return;
    }

    if (!existe_dans_base(ip_a_supprimer, masque_a_supprimer)) {
        printf("L'adresse IP et le masque spécifiés ne sont pas présents dans la base de données.\n");
        return;
    }

    sqlite3 *db;
    int rc;
    rc = sqlite3_open(DB_PATH, &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    const char *sql = "DELETE FROM ip_masque WHERE ip = ? AND masque = ?";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_bind_text(stmt, 1, ip_a_supprimer, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, masque_a_supprimer, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
    } else {
        printf("L'adresse IP et le masque ont été supprimés de la base de données.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


void menu(){
    creer_base_sql();
    char choix;
        do {
            printf("\nMenu:\n");
            printf("a - Ajouter une nouvelle adresse IP.\n");
            printf("l - Liste les adresses IP.\n");
            printf("r - Rechercher par masque de sous-réseau.\n");
            printf("s - Supprimer une adresse IP.\n");
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
                case 'r':
                    recherche_par_masque();
                    break;
                case 's':
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
