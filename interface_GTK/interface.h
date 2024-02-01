#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk/gtk.h>
#include <pango/pangocairo.h>
#include <stdio.h>

// Déclarer les variables globales
extern GtkWidget *window;
extern GtkWidget *fixed;
extern GtkWidget *label;
extern GtkWidget *dialog;
extern GtkWidget *content_area;
extern GtkWidget *entry;

// Déclarer les fonctions
const char *fenetre_input_adresse_ip(GtkWidget *widget, const char *data);
const char *fenetre_input_masque(GtkWidget *widget, const char *data);
void clique(GtkWidget *widget, GdkEventButton *event, gpointer data);
void creation_rectangle(GtkWidget *fixed, int x, int y, int l, int h, const gchar *text);
void menu_interface(int argc, char *argv[]);



#endif // INTERFACE_H
