#include "../includes/catalogue_ip.h"

listAdr addDataLst(listAdr myLst, char *Ip, char *Mask, char * Ipbin, char *Iphex) {
    // printf ("Starting address\n");
    
    element *new_lst = malloc(sizeof(element));
    new_lst->Ip = strdup(Ip);
    new_lst->Mask = strdup(Mask);
    new_lst->binary_Ip = strdup(Ipbin);
    new_lst->Hex_Ip = strdup(Iphex);
    // new_lst->IpNetwork = getIpNetwork(Ip, Mask);
    new_lst->nxt = myLst;
    // displaylist(myLst);
    return new_lst;
}

void displaylist(listAdr myLst) {
    element *tmp = myLst;
    int i = 0;
    while (tmp != NULL) {
        i++;
        printf("%d- Ip: %s\tMask: %s\tBinary: %s\tHexa: %s\n", i, tmp->Ip, tmp->Mask, tmp->binary_Ip, tmp->Hex_Ip);
        // printf ("Network: %s\n", tmp->IpNetwork);
        tmp = tmp->nxt;
    }
    if (i == 0)
        printf ("\033[1;31mLa base de donnée est vide.\033[0m\n");
}

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

void freeList(listAdr myLst) {
    while (myLst != NULL) {
        element *temp = myLst;
        myLst = myLst->nxt;
        free(temp);
    }
}