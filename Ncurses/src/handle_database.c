#include "../includes/my.h"

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
                      "ip_hexa VARCHAR(10) NOT NULL"
                      ");";

    rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Erreur lors de la création de la table : %s\n", sqlite3_errmsg(db));
    else
        printf("La base de données a été créée avec succès.\n");
    sqlite3_close(db);
}


listAdr load_Sql_In_List(listAdr myLst) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    const char *sql = "SELECT * FROM ip_masque";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *ip = sqlite3_column_text(stmt, 1);
        const unsigned char *masque = sqlite3_column_text(stmt, 2);
        const unsigned char *ip_binaire = sqlite3_column_text(stmt, 3);
        const unsigned char *ip_hex = sqlite3_column_text(stmt, 4);
        myLst = addDataLst(myLst, (char *)ip, (char *)masque, (char *)ip_binaire, (char *)ip_hex);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return myLst;
}

char *ajouter_ip(const char *ip, const char *masque, bool graphique) {
    WINDOW *win;
    char *resultats = g_strdup("");
    sqlite3 *db;
    int rc;
    const char *sql_request;

    if (validData_interface((char *) ip, (char *) masque) == 0) {
        if(graphique){
            resultats = g_strdup("Vous devez entrer une IP et un MASQUE.");
            return resultats;
        }
        else{

            TextColored(win, 4, 40, "Vous devez entrer une IP et un MASQUE.", 2);

        }
        
    }

    if (validData_interface((char *) ip, (char *) masque) == false) {

        if(graphique){
        resultats = g_strdup("Merci de rentrer un masque ou une ip valide en X.X.X.X");
        return resultats;
        }
        else{

            TextColored(win, 4, 40, "Merci de rentrer un masque ou une ip valide en X.X.X.X", 2);

        }
    }

    rc = sqlite3_open(DB_PATH, &db);
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
        resultats = g_strdup("Impossible d'ouvrir la base de données.\n");
        return resultats ;
    }
    sql_request = "INSERT INTO ip_masque (ip, masque, ip_binaire, ip_hexa) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql_request, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
        resultats = g_strdup("Erreur lors de la préparation de la requête.\n");
        return resultats ;
    }
    sqlite3_bind_text(stmt, 1, ip, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, masque, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, ip_to_binary((char *)ip), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, ip_to_hex((char *)ip), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
        resultats = g_strdup("Erreur lors de l'exécution de la requête .\n");
        return resultats ;
    }
    else {
        printf("\033[0;32mL'adresse IP et le masque ont été ajoutés à la base de données.\033[0m\n");
        resultats = g_strdup("L'adresse IP et le masque ont été ajoutés à la base de données.\n");
        return resultats;
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
 * @return Un message indiquant le succès ou l'échec de la suppression.
 *         En cas d'erreur, renvoie un message explicatif.
 *         La mémoire allouée est libérée après utilisation.
 */
char *supprimer_ip(const char *ip, const char *masque, bool graphique) {

    char *resultats = g_strdup("");
    sqlite3 *db;
    int rc;
    rc = sqlite3_open(DB_PATH, &db);

    if (validData_interface((char *) ip, (char *) masque) == 0 && graphique == true) {
        resultats = g_strdup("Vous devez entrer une IP et un MASQUE.");
        return resultats;
    }

    if (validData_interface((char *) ip, (char *) masque) == false && graphique == true) {
        resultats = g_strdup("Merci de rentrer un masque ou une ip valide en X.X.X.X");
        return resultats;
    }
    if (rc) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
        resultats = g_strdup("Impossible d'ouvrir la base de données : %s\n");
        return resultats;
    }

    const char *sql = "DELETE FROM ip_masque WHERE ip = ? AND masque = ?";
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

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
        resultats = g_strdup("Erreur lors de l'exécution de la requête.\n");
        return resultats;
    } else {
        printf("\033[0;32mL'adresse IP et le masque ont été supprimés de la base de données.\033[0m\n");
        resultats = g_strdup("L'adresse IP et le masque ont été supprimés de la base de données.\n");
        return resultats;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return resultats;
}