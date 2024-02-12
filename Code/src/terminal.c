#include "../includes/catalogue_ip.h"

void help() {
    printf ("\t\tVoici les commandes pour utiliser ce programme :\n");
    printf ("\t\t\033[0;32madd <IP> <MASQUE>\033[0m : Ajouter une adresse IP et son MASQUE\n");
    printf ("\t\t\033[0;32msearch <IP> <MASQUE>\033[0m : Filtrer les adresses IP qui appartiennent au même sous-réseau\n");
    printf ("\t\t\033[0;32msearch_net <IP> <MASQUE>\033[0m : Filtrer les adresses IP étant dans le même réseau\n");
    printf ("\t\t\033[0;32mlist\033[0m : Afficher les adresses IP ainsi que leurs formes binaires, hexadécimales et décimales\n");
    printf ("\t\t\033[0;32mdelete <IP> <MASQUE>\033[0m : Supprimer une adresse IP\n");
    printf ("\t\t\033[0;32mncurses\033[0m : Lancer l'interface terminale SDL\n");
    printf ("\t\t\033[0;32mgtk\033[0m : Lancer l'interface graphique SDL\n");
    printf ("\t\t\033[0;32mhelp\033[0m : Display help\n");
    printf ("\t\t\033[0;32mquit\033[0m : Quitter le programme\n");
}

void bad_command() {
    printf ("\033[1;31mCommande invalide. Essaye la commande \" help \".\033[0m\n");
}

listAdr addIp(listAdr myLst, char *ip, char *mask) {
    ajouter_ip(ip, mask, false);
    myLst = addDataLst(myLst, ip, mask, ip_to_binary(ip), ip_to_hex(ip));
    return myLst;
}

listAdr deleteIP(listAdr myLst, char *ip, char *mask) {
    supprimer_ip(ip, mask, false);
    myLst = deleteDataLst(myLst, ip, mask);
    return myLst;
}

