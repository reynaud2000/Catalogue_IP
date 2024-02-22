/**
 * @file
 * @brief Ce fichier contient les fonctions qui vont être utilisés dans l'interface gui et cli.
 */
#include "../includes/catalogue_ip.h"


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
        perror("Erreur lors de la duplication de la chaîne IP");
        printf("Contenu de la chaîne IP : %s\n", ip);
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
 * @param ncurses Un booléen indiquant si l'ajout se fait en mode ncurses.
 * @param fenetre La variable responsable de la fenetre avec ncurses.
 * @return Un message indiquant le succès ou l'échec de l'ajout.
 */
char* ajouter_ip(const char *ip, const char *masque, bool graphique, bool ncurses, WINDOW *fenetre) {
    char ip_saisie[16];
    char masque_saisie[16];
    char *resultats = g_strdup("");

    if (graphique == false && ncurses == false) {
        printf("Entrez une adresse IP : ");
        scanf("%15s", ip_saisie);
        printf("Entrez un masque : ");
        scanf("%15s", masque_saisie);
        
        if (!ip_valide(ip_saisie) || !masque_valide(masque_saisie)) {
            printf("Adresse IP ou masque invalide\n");
            return resultats;
        } 
        if (existe_dans_base(ip_saisie, masque_saisie)) {
            printf("L'adresse IP et le masque sont déjà présents dans la base.\n");
            return resultats;
        }
        ip = ip_saisie;
        masque = masque_saisie;
    }
    
    if (!ip_valide(ip) || !masque_valide(masque)) {
        if (graphique) {
            printf("Adresse IP ou masque invalide.\n");
            resultats = g_strdup("Adresse IP ou masque invalide.\n");
            return resultats;
        }
        else {
            Couleur_Texte(fenetre, 4, 50, "Adresse IP ou masque invalide.\n", COLOR_RED);
            return resultats;
        }
    }

    if (existe_dans_base(ip, masque)) {
        if (graphique) {
            printf("L'adresse IP et le masque sont déjà présents dans la base.\n");
            resultats = g_strdup("L'adresse IP et le masque sont déjà présents dans la base.\n");
            return resultats;
        }
        else {
            Couleur_Texte(fenetre, 4, 50, "L'adresse IP et le masque sont déjà présents dans la base.\n", COLOR_RED);
            return resultats;
        }
    }

    char ip_hexa[16];
    char masque_hexa[16];
    convertir_en_hexa(ip, ip_hexa);
    convertir_en_hexa(masque, masque_hexa);
    
    sqlite3 *db;
    int rc;
    rc = sqlite3_open(DB_PATH, &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
        resultats = g_strdup("Impossible d'ouvrir la base de données.\n");
        return resultats;
    }

    char ip_binaire[36];
    char masque_binaire[36];
    convertir_en_binaire(ip, ip_binaire);
    convertir_en_binaire(masque, masque_binaire);
    
    const char *sql = "INSERT INTO ip_masque (ip, masque, ip_binaire, masque_binaire, ip_hexa, masque_hexa) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
        resultats = g_strdup("Erreur lors de la préparation de la requête.\n");
        return resultats;
    }

    sqlite3_bind_text(stmt, 1, ip, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, masque, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, ip_binaire, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, masque_binaire, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, ip_hexa, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, masque_hexa, -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        if (graphique) {
            fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
            resultats = g_strdup("Erreur lors de l'exécution de la requête.\n");
            return resultats;
        }
        else if (ncurses) {
            Couleur_Texte(fenetre, 4, 50, "Erreur lors de l'exécution de la requête.\n", COLOR_RED);
            return resultats;
        }
        else {
            fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
            return resultats;
        }
    } else {
        if (graphique) {
            printf("L'adresse IP et le masque ont été ajoutés à la base de données.\n");
            resultats = g_strdup("L'adresse IP et le masque ont été ajoutés à la base de données.\n");
            return resultats;
        }
        else if (ncurses) {
            Couleur_Texte(fenetre, 4, 40, "L'adresse IP et le masque ont été ajoutés à la base de données.\n", COLOR_GREEN);
            return resultats;
        }
        else {
            printf("L'adresse IP et le masque ont été ajoutés à la base de données.\n");
            return resultats;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return resultats;
}


/**
 * Vérifie si une adresse IP avec un masque est déjà présente dans la base de données.
 * @param ip L'adresse IP à rechercher dans la base de données.
 * @param masque Le masque à rechercher dans la base de données.
 * @return 1 si l'adresse IP et le masque existent dans la base de données, 0 sinon, -1 en cas d'erreur.
 */
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

/**
 * Liste les adresses IP et leurs informations depuis la base de données.
 * @param graphique Indique si l'affichage est destiné à une interface graphique.
 * @param ncurses Indique si l'affichage est destiné à une interface terminale.
 * @param fenetre La variable responsable de la fenetre avec ncurses.
 * @return Une chaîne de caractères contenant la liste des adresses IP et leurs informations.
 *         En cas d'erreur, renvoie une chaîne vide ou un message d'erreur.
 *         La mémoire allouée doit être libérée après utilisation.
 */
char *lister_ip(bool graphique, bool ncurses, WINDOW *fenetre) {

    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    char *resultats = g_strdup("");

    rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        resultats = g_strdup("Impossible d'ouvrir la base de données");
        return resultats;
    }

    const char *sql = "SELECT * FROM ip_masque";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        resultats = g_strdup("Erreur lors de la préparation de la requête");
        return resultats;
    }

    if (!graphique && !ncurses) {
        printf("Liste des adresses IP :\n");
        printf("------------------------------------------------\n\n");
    } else {
        resultats = g_strconcat(resultats, "Liste des adresses IP :\n", "------------------------------------------------\n", NULL);
    }
    int x = 4, i = 1;
    while (sqlite3_step(stmt) == SQLITE_ROW) {

        const unsigned char *ip = sqlite3_column_text(stmt, 1);
        const unsigned char *masque = sqlite3_column_text(stmt, 2);
        const unsigned char *ip_binaire = sqlite3_column_text(stmt, 3);
        const unsigned char *masque_binaire = sqlite3_column_text(stmt, 4);
        const unsigned char *ip_hexa = sqlite3_column_text(stmt, 5);
        const unsigned char *masque_hexa = sqlite3_column_text(stmt, 6);

        if (graphique) {
            resultats = g_strconcat(resultats, "IP: ", ip, " | Masque: ", masque, "| IP Binaire:", ip_binaire, "| IP Hexa:", ip_hexa, "\n", NULL);
        }
        else if (ncurses) {
            mvwprintw(fenetre, x, 5, "Ip: %s\tMask: %s\tBinary: %s\tHexa: %s\n",ip, masque, ip_binaire, ip_hexa);
            x++;
            i++;
        }
        else {
            printf("IP: %s | Masque: %s | IP Binaire: %s | IP Hexa: %s\n", ip, masque, ip_binaire, ip_hexa);
        } 
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return resultats;
}

/**
 * Supprime une adresse IP et son masque associé de la base de données.
 * @param ip L'adresse IP à supprimer.
 * @param masque Le masque à supprimer.
 * @param graphique Indique si l'affichage est destiné à une interface graphique.
 * @param ncurses Un booléen indiquant si l'ajout se fait en mode ncurses.
 * @param fenetre La variable responsable de la fenetre avec ncurses.
 * @return Un message indiquant le succès ou l'échec de la suppression.
 *         En cas d'erreur, renvoie un message explicatif.
 *         La mémoire allouée est libérée après utilisation.
 */
char * supprimer_ip(const char *ip, const char *masque, bool graphique, bool ncurses, WINDOW *fenetre) {
    char ip_saisie[16];
    char masque_saisie[16];
    char *resultats = g_strdup("");

    if (!graphique && !ncurses) {
        printf("Entrez l'adresse IP à supprimer : ");
        scanf("%15s", ip_saisie);
        printf("Entrez le masque à supprimer : ");
        scanf("%15s", masque_saisie);        

        if (!ip_valide(ip_saisie) || !masque_valide(masque_saisie)) {
            printf("Adresse IP ou masque invalide\n");
            return resultats;
        }
        if (!existe_dans_base(ip_saisie, masque_saisie)) {
            printf("L'adresse IP et le masque spécifiés ne sont pas présents dans la base de données.\n");
            return resultats;
        } 

        ip = ip_saisie;
        masque = masque_saisie;
    }


    if (!ip_valide(ip) || !masque_valide(masque)) {
        if (graphique) {
            printf("Adresse IP ou masque invalide.\n");
            resultats = g_strdup("Adresse IP ou masque invalide.\n");
            return resultats;
        }
        else {
            Couleur_Texte(fenetre, 4, 50, "Adresse IP ou masque invalide.\n", COLOR_RED);
            return resultats;
        }
    }

    if (!existe_dans_base(ip, masque)) {
        if (graphique) {
            printf("L'adresse IP et le masque spécifiés ne sont pas présents dans la base de données.\n");
            resultats = g_strdup("L'adresse IP et le masque spécifiés ne sont pas présents dans la base de données.\n");
            return resultats;
        }
        else {
            Couleur_Texte(fenetre, 4, 40, "L'adresse IP et le masque spécifiés ne sont pas présents dans la base de données.\n", COLOR_RED);
            return resultats;
        }
    }

    sqlite3 *db;
    int rc;
    rc = sqlite3_open(DB_PATH, &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
        resultats = g_strdup("Impossible d'ouvrir la base de données : %s\n");
    }

    const char *sql = "DELETE FROM ip_masque WHERE ip = ? AND masque = ?";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
        resultats = g_strdup("Erreur lors de la préparation de la requête.\n");
    }

    sqlite3_bind_text(stmt, 1, ip, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, masque, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        if (ncurses)
            Couleur_Texte(fenetre, 4, 50, "Erreur lors de l'exécution de la requête.\n", COLOR_RED);
        else
            fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
        resultats = g_strdup("Erreur lors de l'exécution de la requête.\n");
    } else {
        if (ncurses)
            Couleur_Texte(fenetre, 4, 50, "L'adresse IP et le masque ont été supprimés de la base de données.\n", COLOR_GREEN);
        else
            printf("L'adresse IP et le masque ont été supprimés de la base de données.\n");
        resultats = g_strdup("L'adresse IP et le masque ont été supprimés de la base de données.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return resultats;
}

/**
 * Cette fonction prend une adresse IP et un masque sous-réseau au format décimal
 * et retourne l'adresse réseau correspondante.
 *
 * @param ip   L'adresse IP au format décimal pointé.
 * @param masque Le masque de sous-réseau au format décimal pointé.
 * @return     L'adresse réseau résultante au format décimal pointé.
 */
char *ip_masque_sous_reseau(char *ip, char *masque) {
    int Ip1, Ip2, Ip3, Ip4;
    int mask1, mask2, mask3, mask4;
    char IpNetwork[16];

    sscanf(ip, "%d.%d.%d.%d", &Ip1, &Ip2, &Ip3, &Ip4);
    sscanf(masque, "%d.%d.%d.%d", &mask1, &mask2, &mask3, &mask4);
    sprintf (IpNetwork, "%d.%d.%d.%d", Ip1 & mask1, Ip2 & mask2, Ip3 & mask3, Ip4 & mask4);
    return (strdup(IpNetwork));
}

/**
 * Recherche les adresses IP dans la base de données en fonction de l'adresse IP et du masque spécifiés.
 *
 * @param graphique Indique si l'interface graphique est utilisée.
 * @param ncurses Un booléen indiquant si l'interface ncurses est utilisée.
 * @param fenetre La variable responsable de la fenêtre avec ncurses.
 * @param ip_add L'adresse IP à utiliser pour le filtrage.
 * @param mask Le masque de sous-réseau à utiliser pour le filtrage.
 * @return Une chaîne de caractères contenant les résultats du filtrage.
 *         Si aucune adresse IP n'est trouvée, une chaîne vide est retournée.
 *         En cas d'erreur, un message d'erreur approprié est retourné.
 */
char *recherche_par_masque(bool graphique, bool ncurses, WINDOW *fenetre, const char *ip_a, const char *masque_a) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    char *resultats = g_strdup("");
    char ip_saisie[16];
    char masque_saisie[16];
    
    if(!graphique && !ncurses) {   
        printf("Entrez une adresse IP : ");
        scanf("%15s", ip_saisie);
        printf("Entrez un masque : ");
        scanf("%15s", masque_saisie);
        
        if (!ip_valide(ip_saisie) || !masque_valide(masque_saisie)) {
            printf("Adresse IP ou masque invalide.\n");
            return resultats;
        } 
        
        ip_a = ip_saisie;
        masque_a = masque_saisie;
    }
    if (!ip_valide(ip_a) || !masque_valide(masque_a)) {
        if (graphique) {
            printf("Adresse IP ou masque invalide.\n");
            resultats = g_strdup("Adresse IP ou masque invalide.\n");
            return resultats;
        }
        else {
            Couleur_Texte(fenetre, 4, 50, "Adresse IP ou masque invalide.\n", COLOR_RED);
            return resultats;
        }
    }
    
    rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        resultats = g_strdup("Impossible d'ouvrir la base de données");
        return resultats;
    }

    const char *sql = "SELECT * FROM ip_masque";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        resultats = g_strdup("Erreur lors de la préparation de la requête");
        return resultats;
    }
    
    if (!graphique && !ncurses) {
        printf("\nListe des adresses IP dans le même sous réseau que %s | %s:\n", ip_saisie,masque_saisie);
        printf("------------------------------------------------\n\n");
    } else {
        resultats = g_strconcat(resultats, "Liste des adresses IP :\n", "------------------------------------------------\n", NULL);
    }
    int x = 4, i = 1;
    while (sqlite3_step(stmt) == SQLITE_ROW) {

        const unsigned char *ip = sqlite3_column_text(stmt, 1);
        const unsigned char *masque = sqlite3_column_text(stmt, 2);
        const unsigned char *ip_binaire = sqlite3_column_text(stmt, 3);
        const unsigned char *ip_hex = sqlite3_column_text(stmt, 4);
        if (strcmp(ip_masque_sous_reseau((char *)ip, (char *)masque), ip_masque_sous_reseau((char *)ip_a, (char *)masque_a)) == 0) {
            
            if(graphique){
                resultats = g_strconcat(resultats,"IP: ", ip, "\n", NULL);
            }
            else if (ncurses) {
                mvwprintw(fenetre, x, 50, "Ip: %s\n", ip);
                x++;
                i++;
            }
            else{
                printf("IP: %s\n", ip);
            }
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return resultats;
}