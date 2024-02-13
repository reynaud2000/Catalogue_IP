/**
 * @file
 * @brief Ce fichier contient les fonctions pour gérer le main.
 */
#include "../includes/catalogue_ip.h"

int main(int argc, char *argv[]){
    listAdr myLst = NULL;
    Loop_Menu(myLst);
    freeList(myLst);
    return 0;
}