/**
 * @file
 * @brief Ce fichier contient les fonctions pour gérer les commandes de l'interface terminal.
 */
#include "../includes/catalogue_ip.h"

/**
 * Cette fonction affiche les commandes disponibles pour utiliser le programme.
 */
void help() {
    printf ("\t\tVoici les commandes pour utiliser ce programme :\n");
    printf ("\t\t\033[0;32madd <IP> <MASQUE>\033[0m : Ajouter une adresse IP et son MASQUE\n");
    printf ("\t\t\033[0;32msearch <IP> <MASQUE>\033[0m : Filtrer les adresses IP qui appartiennent au même sous-réseau\n");
    printf ("\t\t\033[0;32msearch_net <IP> <MASQUE>\033[0m : Filtrer les adresses IP étant dans le même réseau\n");
    printf ("\t\t\033[0;32mlist\033[0m : Afficher les adresses IP ainsi que leurs formes binaires, hexadécimales et décimales\n");
    printf ("\t\t\033[0;32mdelete <IP> <MASQUE>\033[0m : Supprimer une adresse IP\n");
    printf ("\t\t\033[0;32mncurses\033[0m : Lancer l'interface terminale\n");
    printf ("\t\t\033[0;32mgtk\033[0m : Lancer l'interface graphique\n");
    printf ("\t\t\033[0;32mhelp\033[0m : Aide\n");
    printf ("\t\t\033[0;32mquit\033[0m : Quitter le programme\n");
}
/**
 * Cette fonction est appelée lorsque l'utilisateur entre une commande invalide.
 */
void bad_command() {
    printf ("\033[1;31mCommande invalide. Essaye la commande \" help \".\033[0m\n");
}

/**
 * Cette fonction ajoute une nouvelle adresse IP à la liste des adresses IP.
 *
 * @param myLst La liste actuelle des adresses IP.
 * @param ip L'adresse IP à ajouter.
 * @param mask Le masque associé à l'adresse IP.
 * @return La liste mise à jour des adresses IP.
 */
listAdr addIp(listAdr myLst, char *ip, char *mask) {
  
    ajouter_ip(ip, mask, false);
    myLst = addDataLst(myLst, ip, mask, ip_en_binaire(ip), ip_en_hexa(ip));
    return myLst;
}

/**
 * Cette fonction supprime une adresse IP de la liste des adresses IP.
 *
 * @param myLst La liste actuelle des adresses IP.
 * @param ip L'adresse IP à supprimer.
 * @param mask Le masque associé à l'adresse IP à supprimer.
 * @return La liste mise à jour des adresses IP.
 */
listAdr deleteIP(listAdr myLst, char *ip, char *mask) {
    supprimer_ip(ip, mask, false);
    myLst = deleteDataLst(myLst, ip, mask);
    return myLst;
}