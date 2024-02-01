#include <stdbool.h>
#ifndef CATALOGUE_IP_PH
#define CATALOGUE_IP_H

int masque_valide(const char *masque);
int ip_0_255(const char *partie);
int ip_valide(const char *ip);
int ajouter_ip(const char *ip, const char *masque, bool graphique);
void lister_ip();
int masque_similaire(const char *ip1, const char *ip2, const char *masque);
void recherche_par_masque();
void supprimer_ip();
void afficher_representation_ip(const char *ip);
void menu(int argc, char *argv[]);

#endif
