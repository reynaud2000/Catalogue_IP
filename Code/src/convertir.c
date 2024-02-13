/**
 * @file
 * @brief Ce fichier contient les fonctions pour convertir les ip en binaire et en hexa.
 */
#include "../includes/catalogue_ip.h"

/**
 * Convertit une adresse IP de format décimal en format binaire.
 *
 * @param ip L'adresse IP au format décimal à convertir.
 * @return Un pointeur vers la chaîne de caractères représentant l'adresse IP convertie en binaire.
 */
char *ip_en_binaire(char *ip) {
    char resultat[40];
    int Ip1, Ip2, Ip3, Ip4;
    sscanf(ip, "%d.%d.%d.%d", &Ip1, &Ip2, &Ip3, &Ip4);
    snprintf(resultat, sizeof(resultat), "%08b.%08b.%08b.%08b\t", Ip1, Ip2, Ip3, Ip4);
    return strdup(resultat);
}

/**
 * Convertit une adresse IP de format décimal en format hexadécimal.
 *
 * @param ip L'adresse IP au format décimal à convertir.
 * @return Un pointeur vers la chaîne de caractères représentant l'adresse IP convertie en hexadécimal.
 */
char* ip_en_hexa(char *ip) {
    char resultat[16];
    int Ip1, Ip2, Ip3, Ip4;
    sscanf(ip, "%d.%d.%d.%d", &Ip1, &Ip2, &Ip3, &Ip4);
    snprintf(resultat, sizeof(resultat), "%X.%X.%X.%X\n", Ip1, Ip2, Ip3,Ip4);
    return strdup(resultat);
}