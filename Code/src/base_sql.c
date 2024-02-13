/**
 * @file
 * @brief Ce fichier contient les fonctions pour gérer les adresses IP dans une base de données SQLite.
 */

#include "../includes/catalogue_ip.h"

/**
 * La fonction prend en charge la création de la table 'ip_masque' avec les colonnes suivantes :
 * - id : clé primaire auto-incrémentée
 * - ip : chaîne de caractères représentant une adresse IP
 * - masque : chaîne de caractères représentant un masque de sous-réseau
 * - ip_binaire : chaîne de caractères représentant l'adresse IP en format binaire
 * - ip_hexa : chaîne de caractères représentant l'adresse IP en format hexadécimal
 * Si la création réussit, un message de confirmation est affiché sinon un message d'erreur est affiché.
 *
 * @param Aucun paramètre n'est passé, mais la fonction utilise le chemin de la base de données (DB_PATH) défini globalement.
 * @return La fonction ne retourne aucune valeur, mais elle exécute les actions décrites ci-dessous.
 */
void creer_base_sql() {
    sqlite3 *base_sql;
    int resultat;
    resultat = sqlite3_open(DB_PATH, &base_sql);
    if (resultat) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        exit(1);
    }
    const char *requete_sql = "CREATE TABLE IF NOT EXISTS ip_masque ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "ip VARCHAR(15) NOT NULL,"
                      "masque VARCHAR(15) NOT NULL,"
                      "ip_binaire VARCHAR(35) NOT NULL,"
                      "ip_hexa VARCHAR(10) NOT NULL"
                      ");";

    resultat = sqlite3_exec(base_sql, requete_sql, NULL, 0, NULL);
    if (resultat != SQLITE_OK)
        fprintf(stderr, "Erreur lors de la création de la table : %s\n", sqlite3_errmsg(base_sql));
    else
        printf("La base de données a été créée avec succès.\n");
    sqlite3_close(base_sql);
}
/**
 * Cette fonction récupère les données de la table 'ip_masque' et les charge dans une liste chaînée.
 * Si une erreur survient lors de l'ouverture de la base de données ou de la préparation de la requête,
 * un message d'erreur est affiché.
 *
 * @param liste La liste chaînée de structures listAdr dans laquelle les données seront chargées.
 * @return La liste chaînée mise à jour contenant les données extraites de la base de données.
 */
listAdr sql_en_liste(listAdr liste) {
    sqlite3 *base_sql;
    sqlite3_stmt *instruction;
    int resultats;

    resultats = sqlite3_open(DB_PATH, &base_sql);
    if (resultats != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
    }

    const char *requete_sql = "SELECT * FROM ip_masque";
    resultats = sqlite3_prepare_v2(base_sql, requete_sql, -1, &instruction, NULL);
    if (resultats != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
    }
    while (sqlite3_step(instruction) == SQLITE_ROW) {
        const unsigned char *ip = sqlite3_column_text(instruction, 1);
        const unsigned char *masque = sqlite3_column_text(instruction, 2);
        const unsigned char *ip_binaire = sqlite3_column_text(instruction, 3);
        const unsigned char *ip_hexa = sqlite3_column_text(instruction, 4);
        liste = addDataLst(liste, (char *)ip, (char *)masque, (char *)ip_binaire, (char *)ip_hexa);
    }
    sqlite3_finalize(instruction);
    sqlite3_close(base_sql);
    return liste;
}
/**
 * Cette fonction ajoute une ip et un masque dans la table 'ip_masque'.
 * Si les données ne sont pas valides, elle retourne un message d'erreur approprié.
 * 
 * @param ip L'adresse IP à ajouter à la base de données.
 * @param masque Le masque de sous-réseau à ajouter à la base de données.
 * @param graphique Un booléen indiquant si le message d'erreur ou de succès doit être affiché graphiquement.
 * @return Un message indiquant le succès ou l'échec de l'opération d'ajout dans la base de données.
 */
char *ajouter_ip(const char *ip, const char *masque, bool graphique) {
    char *resultats = g_strdup("");
    sqlite3 *base_sql;
    int resultat;
    const char *requete_sql;
    resultat = sqlite3_open(DB_PATH, &base_sql);


    if (validData_i((char *) ip, (char *) masque) == false) {
        if(graphique){
            resultats = g_strdup("Merci de rentrer un masque ou une ip valide en X.X.X.X");
            return resultats;
        }
        else{
            printf("Merci de rentrer un masque ou une ip valide en X.X.X.X\n");
        }
    
    if (resultat) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        exit(1);
        resultats = g_strdup("Impossible d'ouvrir la base de données.\n");
        return resultats ;
    }
    requete_sql = "INSERT INTO ip_masque (ip, masque, ip_binaire, ip_hexa) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    resultat = sqlite3_prepare_v2(base_sql, requete_sql, -1, &stmt, NULL);
    if (resultat != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        exit(1);
        resultats = g_strdup("Erreur lors de la préparation de la requête.\n");
        return resultats ;
    }
    sqlite3_bind_text(stmt, 1, ip, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, masque, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, ip_en_binaire((char *)ip), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, ip_en_hexa((char *)ip), -1, SQLITE_STATIC);
    resultat = sqlite3_step(stmt);
    if (resultat != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(base_sql));
        resultats = g_strdup("Erreur lors de l'exécution de la requête .\n");
        return resultats ;
    }
    else {
        printf("\033[0;32mL'adresse IP et le masque ont été ajoutés à la base de données.\033[0m\n");
        resultats = g_strdup("L'adresse IP et le masque ont été ajoutés à la base de données.\n");
        return resultats;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(base_sql);
    return resultats;
    }
}

/**
 * Supprime une adresse IP et son masque spécifié de la base de données.
 * 
 * @param ip L'adresse IP à supprimer.
 * @param masque Le masque à supprimer.
 * @param graphique Indique si l'affichage est destiné à une interface graphique.
 * @return Un message indiquant le succès ou l'échec de la suppression.
 */
char *supprimer_ip(const char *ip, const char *masque, bool graphique) {
    char *resultats = g_strdup("");
    sqlite3 *base_sql;
    int resultat;

    resultat = sqlite3_open(DB_PATH, &base_sql);
  
    if (validData_i((char *) ip, (char *) masque) == false && graphique == true) {
        resultats = g_strdup("Merci de rentrer un masque ou une ip valide en X.X.X.X");
        return resultats;
    }

    if (resultat) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        exit(1);
        resultats = g_strdup("Impossible d'ouvrir la base de données : %s\n");
        return resultats;
    }

    const char *requete_sql = "DELETE FROM ip_masque WHERE ip = ? AND masque = ?";
    sqlite3_stmt *instruction;

    resultat = sqlite3_prepare_v2(base_sql, requete_sql, -1, &instruction, NULL);
    if (resultat != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(base_sql));
        sqlite3_close(base_sql);
        exit(1);
        resultats = g_strdup("Erreur lors de la préparation de la requête.\n");
        return resultats;
    }

    sqlite3_bind_text(instruction, 1, ip, -1, SQLITE_STATIC);
    sqlite3_bind_text(instruction, 2, masque, -1, SQLITE_STATIC);

    resultat = sqlite3_step(instruction);
    if (resultat != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(base_sql));
        resultats = g_strdup("Erreur lors de l'exécution de la requête.\n");
        return resultats;
    } else {
        printf("\033[0;32mL'adresse IP et le masque ont été supprimés de la base de données.\033[0m\n");
        resultats = g_strdup("L'adresse IP et le masque ont été supprimés de la base de données.\n");
        return resultats;
    }

    sqlite3_finalize(instruction);
    sqlite3_close(base_sql);
    return resultats;
}