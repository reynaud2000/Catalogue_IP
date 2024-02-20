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
    printf ("\t\t\033[0;32ma\033[0m : Ajouter une nouvelle adresse IP.\n");
    printf ("\t\t\033[0;32ms\033[0m : Supprimer une adresse IP.\n");
    printf ("\t\t\033[0;32ml\033[0m : Lister les adresses IP.\n");
    printf ("\t\t\033[0;32mr\033[0m : Rechercher par masque de sous-réseau.\n");
    printf ("\t\t\033[0;32mgui\033[0m : Lancer l'interface graphique\n");
    printf ("\t\t\033[0;32maide\033[0m : Aide\n");
    printf ("\t\t\033[0;32mq\033[0m : Quitter.\n");
}

/**
 * Cette fonction crée une boucle interactive pour gérer les commandes de l'utilisateur.
 * 
 * @param myLst La liste chaînée contenant les adresses IP à gérer.
 */
void menu_ncurses() {
    char *ip = NULL;
    char *masque = NULL;
    creer_base_sql();
    char *line = NULL, *name;
    printf("Quel est votre pseudo d'informaticien?  ");
    name = get();
    printf("Bienvenue \033[0;34m%s\033[0m !!!!!\nConsultez les aides en lancant la commande \" aide \"\n", name);
    while (1) {
        printf("\033[0;34m%s\033[0m# ", name);
        line = get();
        char **command_line = my_str_to_word_array(line);
        if (strcmp(command_line[0], "q") == 0) {
            printf("À la prochaine %s !!!!!\n", name);
            free(line);
            break;
        }
        else if (strcmp(command_line[0], "aide") == 0){
            Aide();
        }
        else if (strcmp(command_line[0], "a") == 0){
            ajouter_ip(ip,masque,false);
        }
        else if (strcmp(command_line[0], "s") == 0){
            supprimer_ip(ip,masque,false);
        }
        else if (strcmp(command_line[0], "l") == 0){
            lister_ip(false);
        }
        else if (strcmp(command_line[0], "r") == 0){
            recherche_par_masque(false,ip,masque);
        }
        else if (strcmp(command_line[0], "gui") == 0) {
            menu_interface();
        }
        else
            printf("\033[1;31mCommande invalide. Essaye la commande \" aide \".\033[0m\n");
            free(line);
            free_2d_array(command_line);
    }
    free(name);
}