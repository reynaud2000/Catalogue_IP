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
void Couleur_Texte(WINDOW *win, int x, int y, char *texte, int color) {
    wattron(win, COLOR_PAIR(color));
    mvwprintw(win, x, y, "%s", texte);
    wattroff(win, COLOR_PAIR(color));
}

/**
 * Cette fonction lit une ligne depuis l'entrée standard (stdin) et la stocke dans un tampon de caractères.
 *
 * @return La ligne lue depuis stdin (sans le caractère de saut de ligne à la fin), ou NULL en cas d'erreur.
 */
char *get(void)
{
    char *tab = NULL;
    size_t len = 0;
    int n = getline(&tab, &len, stdin);

    if (n == -1)
        return (NULL);
    tab[n - 1] = '\0';
    return (tab);
}
/**
 * Cette fonction calcule la longueur d'une chaîne de caractères.
 *
 * @param str La chaîne de caractères dont on veut calculer la longueur.
 * @return    La longueur de la chaîne de caractères.
 */
int my_strlen(char const *str)
{
    int p;
    for (p = 0; str[p] != '\0'; p++);
    return (p);
}

void free_2d_array(char **tab)
{
    for (int i = 0; tab[i]!= NULL; i++)
        free(tab[i]);
}
/**
 * Compte le nombre de mots dans une chaîne de caractères.
 *
 * @param str La chaîne de caractères à analyser.
 * @return Le nombre de mots dans la chaîne.
 */
int number(char const *str)
{
    int a = 0;

    for (int i = 0; str[i]; i++)
        if (str[i] == ' ')
            a++;
    return (a + 1);
}
/**
 * Convertit une chaîne de caractères en tableau de mots.
 *
 * @param str La chaîne de caractères à convertir.
 * @return Un tableau de mots.
 */
char **my_str_to_word_array(char const *str)
{
    int comp = 0, p = 0;
    int a = 0, i = 0, b = number(str);
    char **buffer = malloc (sizeof(char *) * (number(str) + 2));

    for (a = 0; a - 1 != my_strlen(str); a++, p++) {
        for (; str[a] != ' ' && a < my_strlen(str); a++)
            comp++;
        buffer[p] = malloc (sizeof(char) * comp + 1);
        comp = 0;
    }
    p = 0;
    comp = 0;
    for (a = 0; i - 1 != my_strlen(str); buffer[a][p] = '\0', a++, p++, i++)
        for (p = 0; str[i] != ' ' && i < my_strlen(str); p++, i++)
            buffer[a][p] = str[i];
    buffer[a] = NULL;
    return (buffer);
}

/**
 * Cette fonction affiche les commandes disponibles pour utiliser le programme.
 */
void Aide() {
    printf ("\t\tVoici les commandes pour utiliser ce programme :\n");
    printf ("\t\t\033[0;32mgui\033[0m : Lancer l'interface graphique\n");
    printf ("\t\t\033[0;32mcli\033[0m : Lancer l'interface terminal\n");
    printf ("\t\t\033[0;32maide\033[0m : Aide\n");
    printf ("\t\t\033[0;32mq\033[0m : Quitter.\n");
}

/**
 * Cette fonction affiche un guide d'utilisation des commandes pour le programme dans une fenêtre spécifiée de l'interface NCurses.
 *
 * @param fenetre La fenêtre dans laquelle afficher le guide d'utilisation des commandes.
 */
void Aide_ncurses(WINDOW *fenetre) { 
    mvwprintw (fenetre, 5, 20, "Voici les commandes pour utiliser ce programme :\n");
    mvwprintw (fenetre, 6, 20, "a <IP> <MASK> : Ajouter une adresse IP et son MASQUE\n");
    mvwprintw (fenetre, 7, 20, "r <IP> <MASK> : Filtrer les adresses IP qui appartiennent au même sous-réseau\n");
    mvwprintw (fenetre, 8, 20, "l : Afficher les adresses IP ainsi que leurs formes binaires, hexadécimales et décimales\n");
    mvwprintw (fenetre, 9, 20, "s <IP> <MASK> : Supprimer une adresse IP\n");
    mvwprintw (fenetre, 10, 20, "aide : L'aide\n");
    mvwprintw (fenetre, 11, 20, "q : Quitter le programme\n");
}

/**
 * Cette fonction affiche un message d'erreur indiquant une commande invalide dans une fenêtre spécifiée de l'interface NCurses.
 *
 * @param fenêtre La fenêtre dans laquelle le message d'erreur sera affiché.
 */
void Mauvaise_commande_ncurses(WINDOW *fenetre) {
    Couleur_Texte(fenetre, 4, 40, "Commande invalide. Essaye la commande \" aide \".\n", 1);
}

/**
 * Cette fonction affiche un message de bienvenue personnalisé dans le mode ncurses.
 *
 * @param nom Le nom de l'utilisateur à inclure dans le message de bienvenue.
 */
void Bienvenue_ncurses(char *nom) {
    int hauteur, largeur;
    getmaxyx(stdscr, hauteur, largeur);
    mvprintw(hauteur/2, largeur/2 - strlen(nom) - 20, "Bienvenu dans le mode ncurses %s!!!", nom);
    getch();
}

/**
 * Cette fonction lance l'interface utilisateur en mode ncurses pour interagir avec les données des adresses IP.
 *
 * @param nom Le nom de l'utilisateur à afficher dans le message de bienvenue.
 * @param myLst La liste des adresses IP à manipuler.
 * @param fenetre La variable responsable de la fenêtre avec ncurses.
 * @return La liste des adresses IP après les modifications éventuelles.
 */
void Lancer_Ncurses(char *nom, WINDOW *fenetre) {
    initscr();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    Bienvenue_ncurses(nom);
    int i = 2;
    char entrer[60];
    int hauteur, largeur;
    getmaxyx(stdscr, hauteur, largeur);
    fenetre = newwin(20, 130, 8, 10);
    box(fenetre, 0, 0);
    refresh();  
    wrefresh(fenetre);
    while(1) {
        mvwprintw(fenetre, i, (125 - 25) / 2, "Entrez votre commande : ");
        mvwgetstr(fenetre, i + 1, (125 - 25) / 2, entrer);
        char **mots = my_str_to_word_array(entrer);
        if (strcmp(mots[0], "q") == 0) {
            wrefresh(fenetre);
            werase(fenetre);
            break;
        }
        else if (strcmp(mots[0], "aide") == 0) {
            werase(fenetre);
            box(fenetre, 0, 0);
            Aide_ncurses(fenetre);
            wrefresh(fenetre);
            refresh();
        }
        else if (strcmp(mots[0], "a") == 0) {
            werase(fenetre);
            box(fenetre, 0, 0);
            if (number(entrer) != 3)
                Couleur_Texte(fenetre, 4, 45, "Vous devez entrer une ip et un masque.\n", COLOR_RED);
            else
                ajouter_ip(mots[1], mots[2], false, true, fenetre);
            wrefresh(fenetre);
            refresh();
        }
        else if (strcmp(mots[0], "r") == 0) {
            werase(fenetre);
            box(fenetre, 0, 0);
            if (number(entrer) != 3)
                Couleur_Texte(fenetre, 4, 45, "Vous devez entrer une ip et un masque.\n", COLOR_RED);
            else
                recherche_par_masque(false, true, fenetre, mots[1], mots[2]);
            wrefresh(fenetre);
            refresh();
        }
        else if (strcmp(mots[0], "l") == 0) {
            werase(fenetre);
            box(fenetre, 0, 0);
            lister_ip(false, true, fenetre);
            wrefresh(fenetre);
            refresh();
        }
        else if (strcmp(mots[0], "s") == 0) {
            werase(fenetre);
            if (number(entrer) != 3)
                Couleur_Texte(fenetre, 4, 45, "Vous devez entrer une ip et un masque.\n", COLOR_RED);
            else
                supprimer_ip(mots[1], mots[2], false, true, fenetre);
            wrefresh(fenetre);
            refresh();
        }
        else {
            werase(fenetre);
            box(fenetre, 0, 0);
            Mauvaise_commande_ncurses(fenetre);
            wrefresh(fenetre);
            refresh();
        }
    }
    wrefresh(fenetre);
    werase(fenetre);
    delwin(fenetre);
    endwin();
}


/**
 * Cette fonction crée une boucle interactive pour gérer les commandes de l'utilisateur.
 * 
 * @param myLst La liste chaînée contenant les adresses IP à gérer.
 */
void menu_ncurses() {
    char *ip = NULL;
    char *masque = NULL;
    WINDOW *fenetre = newwin(20, 130, 8, 10);    creer_base_sql();
    char *line = NULL, *nom;
    printf("Quel est votre pseudo d'informaticien?  ");
    nom = get();
    printf("Bienvenue \033[0;34m%s\033[0m !!!!!\nConsultez les aides en lancant la commande \" aide \"\n", nom);
    while (1) {
        printf("\033[0;34m%s\033[0m# ", nom);
        line = get();
        char **command_line = my_str_to_word_array(line);
        if (strcmp(command_line[0], "q") == 0) {
            printf("À la prochaine %s !!!!!\n", nom);
            free(line);
            break;
        }
        else if (strcmp(command_line[0], "aide") == 0){
            Aide();
        }
        else if (strcmp(command_line[0], "gui") == 0) {
            menu_interface();
        }
        else if (strcmp(command_line[0], "cli") == 0) {
            Lancer_Ncurses(nom, fenetre);
        }
        else
            printf("\033[1;31mCommande invalide. Essaye la commande \" aide \".\033[0m\n");
            free(line);
            free_2d_array(command_line);
    }
    free(nom);
}