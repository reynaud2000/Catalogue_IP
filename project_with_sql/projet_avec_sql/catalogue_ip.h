#ifndef CATALOGUE_I_PH
#define CATALOGUE_IP_H

int masque_valide(const char *masque);
int ip_0_255(const char *partie);
int ip_valide(const char *ip);
int existe_dans_base(const char *ip, const char *masque);
void convertir_en_binaire(const char *adresse_ip, char *adresse_ip_binaire);
void convertir_en_hexa(const char *adresse_ip, char *adresse_ip_hexa);
void ajouter_ip();
void lister_ip();
int masque_similaire(const char *ip1, const char *ip2, const char *masque);
void recherche_par_masque();
void supprimer_ip();
void afficher_representation_ip(const char *ip);
void menu();

#endif
