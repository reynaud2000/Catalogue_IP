/**
 * @file
 * @brief Ce fichier contient tous les prototypes des fonctions.
 */
#ifndef CATALOGUE_IP
#define CATALOGUE_IP
#include <ctype.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <ncurses.h>
#include <pango/pangocairo.h>
#define DB_PATH "catalogue_ip.db"

extern GtkWidget *window;
extern GtkWidget *fixed;
extern GtkWidget *label;
extern GtkWidget *dialog;
extern GtkWidget *content_area;
extern GtkWidget *entry;


//ncurses.c
void menu_ncurses();
void Aide();
void Bienvenue(char *name);
char **my_str_to_word_array(char const *str);
int number(char const *str);
void free_2d_array(char **tab);
int my_strlen(char const *str);
char *get(void);
void TextColored(WINDOW *win, int x, int y, char *text, int color);

//interface.c
int menu_interface();
void creation_rectangle(GtkWidget *fixe, int x, int y, int l, int h, const gchar *text);
void clique(GtkWidget *widget, GdkEventButton *evenement, gpointer donnee);
void sup_ip(GtkWidget *widget);
void filtrer_par_masque(GtkWidget *widget);
void ajout_ip(GtkWidget *widget);
void affiche_popup(char *resultats);
const char *fenetre_masque(GtkWidget *widget, const char *donnee);
const char *fenetre_adresse_ip(GtkWidget *widget, const char *donnee);

//catalogue_ip.c
int masque_valide(const char *masque);
int ip_0_255(const char *partie);
int ip_valide(const char *ip);
void creer_base_sql();
int existe_dans_base(const char *ip, const char *masque);
void convertir_en_binaire(const char *adresse, char *binaire);
void convertir_en_hexa(const char *adresse_ip, char *adresse_ip_hexa);
char* ajouter_ip(const char *ip, const char *masque, bool graphique);
char *lister_ip(bool graphique);
char * supprimer_ip(const char *ip, const char *masque, bool graphique);
char *recherche_par_masque(bool graphique, const char *ip, const char *masque);
char *ip_masque_sous_reseau(char *ip, char *masque);

#endif 