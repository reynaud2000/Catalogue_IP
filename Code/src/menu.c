/**
 * @file
 * @brief Ce fichier contient les fonctions pour gérer le menu de l'interface terminal.
 */
#include "../includes/catalogue_ip.h"

/**
 * Cette fonction crée une boucle interactive pour gérer les commandes de l'utilisateur.
 * Elle initialise d'abord la base de données SQL et charge les données dans la liste chaînée 'myLst'.
 * Ensuite, elle invite l'utilisateur à saisir son nom.
 * Elle affiche un message de bienvenue avec le nom de l'utilisateur et fournit des instructions d'utilisation.
 * Elle entre ensuite dans une boucle infinie où elle attend les commandes de l'utilisateur.
 *
 * @param myLst La liste chaînée contenant les adresses IP à gérer.
 */
void Loop_Menu(listAdr myLst) {
    creer_base_sql();
    myLst = sql_en_liste(myLst);
    char *line = NULL, *name;
    printf("Donne ton nom jeune informaticien: ");
    name = get();
    printf("Bienvenue \033[0;34m%s\033[0m !!!!!\nConsultez les aides en lancant la commande \" help \"\n", name);
    while (1) {
        printf("\033[0;34m%s\033[0m# ", name);
        line = get();
        char **command_line = my_str_to_word_array(line);
        if (strcmp(command_line[0], "quit") == 0) {
            printf("À la prochaine %s !!!!!\n", name);
            free(line);
            break;
        }
        else if (strcmp(command_line[0], "help") == 0)
            help();
        
        else if (strcmp(command_line[0], "add") == 0) {
            if (validData(command_line[1], command_line[2], line) == true
            && AlreadyExistLst(myLst, command_line[1], command_line[2]) == false)
                myLst = addIp(myLst, command_line[1], command_line[2]);
        }
        
        else if (strcmp(command_line[0], "search") == 0) {
            if (validData(command_line[1], command_line[2], line) == true)
                filterLst(myLst, command_line[1], command_line[2]);
        }

        else if (strcmp(command_line[0], "search_net") == 0) {
            printf ("Search by net\n");
        }
        else if (strcmp(command_line[0], "list") == 0)
            displaylist(myLst);
        else if (strcmp(command_line[0], "delete") == 0) {
            if (validData(command_line[1], command_line[2], line) == true
            && AlreadyExistLst(myLst, command_line[1], command_line[2]) == true)
                myLst = deleteIP(myLst, command_line[1], command_line[2]);
        }
        else if (strcmp(command_line[0], "ncurses") == 0)
            myLst = launchNurcuses(name, myLst);
        else if (strcmp(command_line[0], "gtk") == 0) {
            menu_interface();
            myLst = NULL;
            myLst = sql_en_liste(myLst);
        }
        else
            bad_command();
        free(line);
        free_2d_array(command_line);
    }
    free(name);
}