/**
 * @file
 * @brief Ce fichier contient les fonctions pour gérer la liste qui récupère les données de la base sql.
 */
#include "../includes/catalogue_ip.h"

/**
 * Cette fonction crée un nouvel élément de liste contenant les données fournies (adresse IP, masque, adresse IP en binaire et adresse IP en hexadécimale).
 *
 * @param myLst La liste existante où le nouvel élément sera ajouté.
 * @param Ip L'adresse IP à ajouter dans le nouvel élément.
 * @param Mask Le masque de sous-réseau à ajouter dans le nouvel élément.
 * @param Ipbin L'adresse IP binaire à ajouter dans le nouvel élément.
 * @param Iphex L'adresse IP hexadécimale à ajouter dans le nouvel élément.
 * @return Un pointeur vers la nouvelle tête de liste contenant le nouvel élément ajouté.
 */
listAdr addDataLst(listAdr myLst, char *Ip, char *Mask, char * Ipbin, char *Iphex) {
    element *new_lst = malloc(sizeof(element));
    new_lst->Ip = strdup(Ip);
    new_lst->Mask = strdup(Mask);
    new_lst->binary_Ip = strdup(Ipbin);
    new_lst->Hex_Ip = strdup(Iphex);
    new_lst->nxt = myLst;
    return new_lst;
}

/**
 * Cette fonction affiche les éléments de la liste chaînée 'myLst' contenant des données sur les adresses IP.
 * 
 * @param myLst La liste chaînée contenant les données sur les adresses IP à afficher.
 */
void displaylist(listAdr myLst) {
    element *tmp = myLst;
    int i = 0;
    while (tmp != NULL) {
        i++;
        printf("%d- Ip: %s\tMask: %s\tBinary: %s\tHexa: %s\n", i, tmp->Ip, tmp->Mask, tmp->binary_Ip, tmp->Hex_Ip);
        tmp = tmp->nxt;
    }
    if (i == 0)
        printf ("\033[1;31mLa base de donnée est vide.\033[0m\n");
}
/**
 * Cette fonction supprime un élément de la liste chaînée 'myLst' contenant les données fournies (adresse IP et masque).
 * 
 * @param myLst La liste chaînée contenant les données sur les adresses IP.
 * @param Ip L'adresse IP de l'élément à supprimer.
 * @param Mask Le masque de sous-réseau de l'élément à supprimer.
 * @return La liste chaînée mise à jour après la suppression de l'élément, ou la liste inchangée si l'élément n'a pas été trouvé.
 */
listAdr deleteDataLst(listAdr myLst, char *Ip, char *Mask) {
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
        printf ("\033[1;31mL'adresse IP n'a pas été supprimé car elle n'existe pas.\033[0m\n");

    return myLst;
}
/**
 * Cette fonction vérifie si une adresse IP avec un masque de sous-réseau donné existe déjà dans la liste chaînée 'myLst'.
 *
 * @param myLst La liste chaînée contenant les données sur les adresses IP à vérifier.
 * @param Ip L'adresse IP à rechercher dans la liste.
 * @param Mask Le masque de sous-réseau à rechercher dans la liste.
 * @return true si l'adresse IP existe déjà dans la liste, false sinon.
 */
bool AlreadyExistLst(listAdr myLst, char *Ip, char *Mask) {
    element *tmp = myLst;
    int find = 0;
    while (tmp != NULL) {
        if (strcmp(Ip, tmp->Ip) == 0 && strcmp(Mask, tmp->Mask) == 0)
            find = 1;
        tmp = tmp->nxt;
    }
    if (find == 0) {
        printf ("\033[1;31mL'adresse IP n'existe pas.\033[0m\n");
        return false;
    }
    else {
        printf ("\033[1;31mL'adresse IP existe déjà.\033[0m\n");
        return true;
    }
}
/**
 * Cette fonction filtre les éléments de la liste chaînée 'myLst' en fonction d'une adresse IP et d'un masque de sous-réseau donnés.
 *
 * @param myLst La liste chaînée contenant les données sur les adresses IP à filtrer.
 * @param Ip L'adresse IP utilisée comme critère de filtrage.
 * @param Mask Le masque de sous-réseau utilisé comme critère de filtrage.
 */
void filterLst(listAdr myLst, char *Ip, char *Mask) {
    element *tmp = myLst;
    int i = 0;
    while (tmp != NULL) {
        if (strcmp(getIpNetwork(Ip, Mask), getIpNetwork(tmp->Ip, Mask)) == 0) {
            i++;
            printf("%d- Ip: %s\tMask: %s\tBinary: %s\tHex: %s", i, tmp->Ip, tmp->Mask, tmp->binary_Ip, tmp->Hex_Ip);
        }
        tmp = tmp->nxt;
    }
    if (i == 0)
        printf ("\033[1;31mAucune adresse IP n'a été trouvé.\033[0m\n");
}

/**
 * Cette fonction libère la mémoire allouée pour tous les éléments de la liste chaînée 'myLst'.
 *
 * @param myLst La liste chaînée dont la mémoire doit être libérée.
 */
void freeList(listAdr myLst) {
    while (myLst != NULL) {
        element *temp = myLst;
        myLst = myLst->nxt;
        free(temp);
    }
}