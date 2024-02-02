#ifndef CATALOGUE_IP_PH
#define CATALOGUE_IP_H
#include <stdbool.h>

int masque_valide(const char *masque);
int ip_0_255(const char *partie);
int ip_valide(const char *ip);
int existe_dans_base(const char *ip, const char *masque);
void convertir_en_binaire(const char *adresse_ip, char *adresse_ip_binaire);
void convertir_en_hexa(const char *adresse_ip, char *adresse_ip_hexa);
int ajouter_ip(const char *ip, const char *masque, bool graphique);
char *lister_ip(bool graphique);
int masque_similaire(const char *ip1, const char *ip2, const char *masque);
void recherche_par_masque();
void supprimer_ip();
void afficher_representation_ip(const char *ip);
void menu(int argc, char *argv[]);

#endif
