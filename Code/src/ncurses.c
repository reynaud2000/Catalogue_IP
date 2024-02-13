/**
 * @file
 * @brief Ce fichier contient les fonctions pour gérer l'interface terminal.
 */
#include "../includes/catalogue_ip.h"

/**
 * Cette fonction affiche du texte coloré dans une fenêtre spécifiée de l'interface NCurses.
 *
 * @param win La fenêtre dans laquelle le texte coloré sera affiché.
 * @param x La position verticale (ligne) du texte dans la fenêtre.
 * @param y La position horizontale (colonne) du texte dans la fenêtre.
 * @param text Le texte à afficher dans la fenêtre.
 * @param color La paire de couleurs à appliquer au texte (définie par des paires de couleurs attribuées via init_pair()).
 */
void TextColored(WINDOW *win, int x, int y, char *text, int color) {
    wattron(win, COLOR_PAIR(color));
    mvwprintw(win, x, y, "%s", text);
    wattroff(win, COLOR_PAIR(color));
}

/**
 * Cette fonction affiche un message de bienvenue personnalisé dans le mode ncurses.
 *
 * @param name Le nom de l'utilisateur à inclure dans le message de bienvenue.
 */
void Welcome(char *name) {
    int height, width;
    getmaxyx(stdscr, height, width);
    mvprintw(height/2, width/2 - strlen(name) - 20, "Bienvenu dans le mode ncurses %s!!!", name);
    getch();
}

/**
 * Cette fonction affiche un message d'erreur indiquant une commande invalide dans une fenêtre spécifiée de l'interface NCurses.
 *
 * @param win La fenêtre dans laquelle le message d'erreur sera affiché.
 */
void bad_command_ncurses(WINDOW *win) {
    TextColored(win, 6, 40, "Commande invalide. Essaye la commande \" help \".\n", 1);
}

/**
 * Cette fonction ajoute une adresse IP avec son masque correspondant à la base de données SQLite
 *
 * @param ip L'adresse IP à ajouter à la base de données.
 * @param masque Le masque de sous-réseau correspondant à l'adresse IP.
 * @param win La fenêtre dans laquelle afficher les résultats de l'opération.
 */
void ajouter_ip_ncurses(const char *ip, const char *masque, WINDOW *win) {
    char *ip_binaire, *ip_hexa;
    sqlite3 *db;
    int rc;
    const char *sql_request;
    rc = sqlite3_open(DB_PATH, &db);
    if (rc) {
        TextColored(win, 4, 40, "Impossible d'ouvrir la base de données", 2);
        sqlite3_close(db);
        exit(1);
    }
    sql_request = "INSERT INTO ip_masque (ip, masque, ip_binaire, ip_hexa) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql_request, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        TextColored(win, 4, 40, "Erreur lors de l'exécution de la requête", 2);
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_bind_text(stmt, 1, ip, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, masque, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, ip_en_binaire((char *)ip), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, ip_en_hexa((char *)ip), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
        TextColored(win, 4, 40, "Erreur lors de l'exécution de la requête", 2);
    else
        TextColored(win, 4, 40, "L'adresse IP et le masque ont été ajoutés à la base de données.", 2);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

/**
 * Cette fonction supprime une adresse IP avec son masque correspondant de la base de données SQLite
 *
 * @param ip L'adresse IP à supprimer de la base de données.
 * @param masque Le masque de sous-réseau correspondant à l'adresse IP à supprimer.
 * @param win La fenêtre dans laquelle afficher les résultats de l'opération.
 */
void supprimer_ip_ncurses(const char *ip, const char *masque, WINDOW *win) {
    sqlite3 *db;
    int rc;
    rc = sqlite3_open(DB_PATH, &db);
    if (rc) {
        TextColored(win, 4, 40, "Impossible d'ouvrir la base de données", 2);
        sqlite3_close(db);
        exit(1);
    }
    const char *sql = "DELETE FROM ip_masque WHERE ip = ? AND masque = ?";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        TextColored(win, 4, 40, "L'adresse IP et le masque ont été ajoutés à la base de données.", 2);
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_bind_text(stmt, 1, ip, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, masque, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
        TextColored(win, 4, 40, "L'adresse IP et le masque ont été ajoutes à la base de données.", 2);
    else
        TextColored(win, 4, 40, "L'adresse IP et le masque ont été supprimes de la base de données.\n", 2);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

/**
 * Cette fonction affiche un guide d'utilisation des commandes pour le programme dans une fenêtre spécifiée de l'interface NCurses.
 *
 * @param win La fenêtre dans laquelle afficher le guide d'utilisation des commandes.
 */
void help_ncurses(WINDOW *win) { 
    mvwprintw (win, 5, 20, "Voici les commandes pour utiliser ce programme :\n");
    mvwprintw (win, 6, 20, "add <IP> <MASK> : Ajouter une adresse IP et son MASQUE\n");
    mvwprintw (win, 7, 20, "search <IP> <MASK> : Filtrer les adresses IP qui appartiennent au même sous-réseau\n");
    mvwprintw (win, 8, 20, "search_net <IP> <MASK> : Filtrer les adresses IP étant dans le même réseau\n");
    mvwprintw (win, 9, 20, "list : Afficher les adresses IP ainsi que leurs formes binaires, hexadécimales et décimales\n");
    mvwprintw (win, 10, 20, "delete <IP> <MASK> : Supprimer une adresse IP\n");
    mvwprintw (win, 11, 20, "gtk : Lancer l'interface graphique\n");
    mvwprintw (win, 12, 20, "help : L'aide\n");
    mvwprintw (win, 13, 20, "quit : Quitter le programme\n");
}
/**
 * Cette fonction affiche la liste des adresses IP avec leurs détails (masque, IP binaire, IP hexadécimale)
 *
 * @param myLst La liste des adresses IP à afficher.
 * @param win La fenêtre dans laquelle afficher la liste des adresses IP.
 */
void displaylist_ncurses(listAdr myLst, WINDOW *win) {
    element *tmp = myLst;
    int i = 0;
    int x = 4;
    while (tmp != NULL) {
        i++;
        mvwprintw(win, x, 5, "%d- Ip: %s\tMask: %s\tBinary: %s\tHexa: %s\n", i, tmp->Ip, tmp->Mask, tmp->binary_Ip, tmp->Hex_Ip);
        tmp = tmp->nxt;
        x++;
    }
    if (i == 0)
        TextColored(win, 5, 48, "La base de donnée est vide.", 1);
}
/**
 * Cette fonction ajoute une nouvelle adresse IP avec ses détails (masque, IP binaire, IP hexadécimale).
 *
 * @param myLst La liste des adresses IP à laquelle ajouter la nouvelle adresse IP.
 * @param Ip L'adresse IP à ajouter.
 * @param Mask Le masque de sous-réseau correspondant à l'adresse IP.
 * @param Ipbin L'adresse IP binaire.
 * @param Iphex L'adresse IP hexadécimale.
 * @param win La fenêtre dans laquelle afficher le message de succès.
 * @return La liste mise à jour avec la nouvelle adresse IP ajoutée en tête.
 */
listAdr addDataLst_ncurses(listAdr myLst, char *Ip, char *Mask, char * Ipbin, char *Iphex, WINDOW *win) {
    element *new_lst = malloc(sizeof(element));
    new_lst->Ip = strdup(Ip);
    new_lst->Mask = strdup(Mask);
    new_lst->binary_Ip = strdup(Ipbin);
    new_lst->Hex_Ip = strdup(Iphex);
    new_lst->nxt = myLst;
    TextColored(win, 6, 40, "L'adresse IP a ete ajoutee avec succes", 1);
    return new_lst;
}
/**
 * Cette fonction supprime une adresse IP avec son masque correspondant de la liste des adresses IP
 *
 * @param myLst La liste des adresses IP à partir de laquelle supprimer l'adresse IP.
 * @param Ip L'adresse IP à supprimer.
 * @param Mask Le masque de sous-réseau correspondant à l'adresse IP à supprimer.
 * @param win La fenêtre dans laquelle afficher le message d'erreur si l'adresse IP n'est pas trouvée.
 * @return La liste mise à jour après la suppression ou sans modification si l'adresse IP n'est pas trouvée.
 */
listAdr deleteIP_ncurses(listAdr myLst, char *Ip, char *Mask, WINDOW *win) {
    element *tmp = myLst;
    element *prev = NULL;
    int find = 0;
    while (tmp!= NULL) {
        if (strcmp(tmp->Ip, Ip) == 0 && strcmp(tmp->Mask, Mask) == 0) {
            if (prev == NULL)
                myLst = tmp->nxt;
            else
                prev->nxt = tmp->nxt;
            free(tmp);
            find = 1;
            break;
        }
        prev = tmp;
        tmp = tmp->nxt;
    }
    if (find == 0)
        TextColored(win, 6, 40, "Commande invalide. Essaye la commande \" help \".\n", 1);
    return myLst;
}

/**
 * Cette fonction filtre les adresses IP dans la liste 'myLst' qui appartiennent au même réseau que l'adresse IP 'Ip'
 *
 * @param myLst La liste des adresses IP à filtrer.
 * @param Ip L'adresse IP à utiliser comme référence pour le filtre.
 * @param Mask Le masque de sous-réseau à utiliser pour le filtrage.
 * @param win La fenêtre dans laquelle afficher les adresses IP filtrées ou le message d'erreur.
 */
void filterLst_ncurses(listAdr myLst, char *Ip, char *Mask, WINDOW *win) {
    element *tmp = myLst;
    int i = 0;
    int x = 4;
    while (tmp != NULL) {
        if (strcmp(getIpNetwork(Ip, Mask), getIpNetwork(tmp->Ip, Mask)) == 0) {
            i++;
            mvwprintw(win, x, 5, "%d- Ip: %s\tMask: %s\tBinary: %s\tHex: %s\n", i, tmp->Ip, tmp->Mask, tmp->binary_Ip, tmp->Hex_Ip);
            x++;
        }
        tmp = tmp->nxt;
    }
    if (i == 0)
        TextColored(win, 4, 40, "Aucune adresse IP n'a été trouvé.", 1);
}
/**
 * Cette fonction vérifie si une adresse IP avec le même masque existe déjà dans la liste 'myLst'.
 *
 * @param myLst La liste des adresses IP à vérifier.
 * @param Ip L'adresse IP à comparer.
 * @param Mask Le masque de sous-réseau à comparer.
 * @param win La fenêtre dans laquelle afficher le message d'erreur ou le message de confirmation.
 * @return true si l'adresse IP existe déjà dans la liste, false sinon.
 */
bool AlreadyExistLst_ncurses(listAdr myLst, char *Ip, char *Mask, WINDOW *win) {
    element *tmp = myLst;
    int find = 0;
    while (tmp != NULL) {
        if (strcmp(Ip, tmp->Ip) == 0 && strcmp(Mask, tmp->Mask) == 0)
            find = 1;
        tmp = tmp->nxt;
    }
    if (find == 0) {
        TextColored(win, 4, 40, "L'adresse IP n'existe pas.", 1);
        return false;
    }
    else {
        TextColored(win, 4, 40, "L'adresse IP existe déjà.", 1);
        return true;
    }
}
/**
 * Cette fonction lance l'interface utilisateur en mode ncurses pour interagir avec les données des adresses IP.
 *
 * @param name Le nom de l'utilisateur à afficher dans le message de bienvenue.
 * @param myLst La liste des adresses IP à manipuler.
 * @return La liste des adresses IP après les modifications éventuelles.
 */
listAdr launchNurcuses(char *name, listAdr myLst) {
    initscr();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    Welcome(name);
    int i = 2;
    char input[60];
    int height, width;
    getmaxyx(stdscr, height, width);
    WINDOW *win = newwin(20, 130, 8, 10);
    box(win, 0, 0);
    refresh();  
    wrefresh(win);
    while(1) {
        mvwprintw(win, i, (125 - 25) / 2, "Entrez votre commande : ");
        mvwgetstr(win, i + 1, (125 - 25) / 2, input);
        char **words = my_str_to_word_array(input);
        if (strcmp(words[0], "quit") == 0) {
            wrefresh(win);
            werase(win);
            break;
        }
        else if (strcmp(words[0], "help") == 0) {
            werase(win);
            box(win, 0, 0);
            help_ncurses(win);
            wrefresh(win);
            refresh();
        }
        else if (strcmp(words[0], "add") == 0) {
            werase(win);
            box(win, 0, 0);
            if (validData_ncurses(words[1], words[2], input, win) == true
            && AlreadyExistLst_ncurses(myLst, words[1], words[2], win) == false) {
                ajouter_ip_ncurses(words[1], words[2], win);
                myLst = addDataLst_ncurses(myLst, words[1], words[2], ip_en_binaire(words[1]), ip_en_hexa(words[1]), win);
            }
            wrefresh(win);
            refresh();
        }
        else if (strcmp(words[0], "search") == 0) {
            werase(win);
            box(win, 0, 0);
            if (validData_ncurses(words[1], words[2], input, win) == true) {
                filterLst_ncurses(myLst, words[1], words[2], win);
            }
            wrefresh(win);
            refresh();
        }
        else if (strcmp(words[0], "list") == 0) {
            werase(win);
            box(win, 0, 0);
            displaylist_ncurses(myLst, win);
            wrefresh(win);
            refresh();
        }
        else if (strcmp(words[0], "delete") == 0) {
            werase(win);
            box(win, 0, 0);
            if (validData_ncurses(words[1], words[2], input, win) == true
            && AlreadyExistLst_ncurses(myLst, words[1], words[2], win) == true) {
                supprimer_ip_ncurses(words[1], words[2], win);
                myLst = deleteIP_ncurses(myLst, words[1], words[2], win);
            }
            wrefresh(win);
            refresh();
        }
        else if (strcmp(words[0], "gtk") == 0) {
            werase(win);
            box(win, 0, 0);
            wrefresh(win);
            refresh();
        }
        else {
            werase(win);
            box(win, 0, 0);
            bad_command_ncurses(win);
            wrefresh(win);
            refresh();
        }
    }
    wrefresh(win);
    werase(win);
    delwin(win);
    endwin();
    return (myLst);
}