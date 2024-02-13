/**
 * @file
 * @brief Ce fichier contient tous les prototypes des fonctions.
 */

#ifndef CATALOGUE_IP
#define CATALOGUE_IP

#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <ncurses.h>
#include <pango/pangocairo.h>
#define DB_PATH "catalogue_ip.db"

extern GtkWidget *fenetre;
extern GtkWidget *fixe;
extern GtkWidget *label;
extern GtkWidget *dialogue;
extern GtkWidget *contenu;
extern GtkWidget *entrer;
typedef struct element
{
    char *Ip;
    char *Mask;
    char *IpNetwork;
    char *binary_Ip;
    char *Hex_Ip;
    struct element *nxt;
}element, *listAdr;

listAdr addDataLst(listAdr myLst, char *Ip, char *Mask, char * Ipbin, char *Iphex);
void displaylist(listAdr myLst);
void freeList(listAdr myLst);
listAdr sql_en_liste(listAdr myLst);
listAdr deleteDataLst(listAdr myLst, char *Ip, char *Mask);
char *get(void);
void help();
void bad_command();
void creer_base_sql();
void Loop_Menu(listAdr myLst);
char **my_str_to_word_array(char const *str);
int my_strlen(char const *str);
int number(char const *str);
void free_2d_array(char **tab);
char *getIpNetwork(char *ip, char *mask);
char* ip_en_hexa(char *ip);
char* ip_en_binaire(char *ip);
char * ajouter_ip(const char *ip, const char *masque, bool graphique);
char *supprimer_ip(const char *ip, const char *masque, bool graphique);
listAdr launchNurcuses(char *name, listAdr myLst);
void TextColored(WINDOW *win, int x, int y, char *text, int color);
listAdr addIp(listAdr myLst, char *ip, char *mask);
listAdr deleteIP(listAdr myLst, char *ip, char *mask);
bool validData(char *ip, char *mask, char *line);
void filterLst(listAdr myLst, char *Ip, char *Mask);
int number(char const *str);
bool AlreadyExistLst(listAdr myLst, char *Ip, char *Mask);
bool validData_ncurses(char *ip, char *mask, char *line, WINDOW *win);
bool verifyNumberMask(int number);
bool verifyNumberIP(int number);
bool validData_i(char *ip, char *mask);
char *filtered_mask(bool graphique, const char *ip_add, const char *mask);
char *lister_ip(bool graphique);
const char *fenetre_adresse_ip(GtkWidget *widget, const char *data);
const char *fenetre_masque(GtkWidget *widget, const char *data);
void affiche_popup(char *resultats);
void ajout_ip(GtkWidget *widget);
void filtrer_par_mask(GtkWidget *widget);
void sup_ip(GtkWidget *widget);
void clique(GtkWidget *widget, GdkEventButton *event, gpointer data);
void creation_rectangle(GtkWidget *fixe, int x, int y, int l, int h, const gchar *text);
int menu_interface();

#endif 