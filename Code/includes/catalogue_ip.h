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

extern GtkWidget *window;
extern GtkWidget *fixed;
extern GtkWidget *label;
extern GtkWidget *dialog;
extern GtkWidget *content_area;
extern GtkWidget *entry;
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
listAdr load_Sql_In_List(listAdr myLst);
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
char* ip_to_hex(char *ip);
char* ip_to_binary(char *ip);
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
int menu_interface();
bool verifyNumberMask(int number);
bool verifyNumberIP(int number);
bool validData_i(char *ip, char *mask);
char *filtered_mask(bool graphique, const char *ip_add, const char *mask);

#endif 