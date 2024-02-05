#include <gtk/gtk.h>
#include <pango/pangocairo.h>
#include <stdio.h>
#include <pthread.h>
#include "interface.h"
#include "../project_with_sql/catalogue_ip.h"

GtkWidget *window = NULL;
GtkWidget *fixed = NULL;
GtkWidget *label = NULL;
GtkWidget *dialog = NULL;
GtkWidget *content_area = NULL;
GtkWidget *entry = NULL;

/**
 * Cette fonction crée une fenêtre de dialogue pour permettre à l'utilisateur
 * de saisir une adresse IP.
 * @param widget Le widget qui a émis le signal pour l'activation de la fonction.
 * @param data Les données associées au signal (unused).
 * @return const char* L'adresse IP saisie par l'utilisateur, ou NULL si l'utilisateur a annulé.
 */
const char *fenetre_input_adresse_ip(GtkWidget *widget, const char *data) {
    const gchar *bouton_ecrire_texte = (const gchar *)data;
    bouton_ecrire_texte = "Ajouter une adresse IP";
    if (strcmp(bouton_ecrire_texte, "Ajouter une adresse IP") == 0) {
        dialog = gtk_dialog_new_with_buttons("Ajouter une adresse IP",
            GTK_WINDOW(window), GTK_DIALOG_MODAL, "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_CANCEL, NULL);
        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        entry = gtk_entry_new();
        gtk_container_add(GTK_CONTAINER(content_area), entry);
        gtk_widget_show_all(dialog);
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        if (response == GTK_RESPONSE_ACCEPT) {

            const char *ip_address = strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
            
            g_print("Adresse IP saisie : %s\n", ip_address);
            gtk_widget_destroy(dialog);
            return ip_address;
        }
        else if (response == GTK_RESPONSE_CANCEL) {
            gtk_widget_destroy(dialog);
            return NULL;
        }
        gtk_widget_destroy(dialog);
        
    }
    return NULL; 
}

/**
 * Cette fonction crée une fenêtre de dialogue permettant à l'utilisateur de saisir un masque.
 * @param widget Le widget qui a émis le signal pour activer la fonction.
 * @param data Les données associées au signal (non utilisées).
 * @return const char* Le masque saisi par l'utilisateur, ou NULL si l'utilisateur a annulé.
 */
const char *fenetre_input_masque(GtkWidget *widget, const char *data) {
    const gchar *bouton_ecrire_texte = (const gchar *)data;
    bouton_ecrire_texte = "Ajouter un masque";
    if (strcmp(bouton_ecrire_texte, "Ajouter un masque") == 0) {
        dialog = gtk_dialog_new_with_buttons("Ajouter un masque",
            GTK_WINDOW(window), GTK_DIALOG_MODAL, "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_CANCEL, NULL);
        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        entry = gtk_entry_new();
        gtk_container_add(GTK_CONTAINER(content_area), entry);
        gtk_widget_show_all(dialog);
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        if (response == GTK_RESPONSE_ACCEPT) {

            const char *masque = strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
            
            g_print("Masque saisie : %s\n", masque);
            gtk_widget_destroy(dialog);
            return masque;
        }
        else if (response == GTK_RESPONSE_CANCEL) {
            gtk_widget_destroy(dialog);
            return NULL;
        }
        gtk_widget_destroy(dialog);
    }
    return NULL; 
}

/**
 * Cette fonction effectue une recherche en utilisant un masque saisi par l'utilisateur.
 * Elle récupère d'abord le masque via une fenêtre de dialogue, puis effectue une recherche
 * avec ce masque et affiche le résultat dans une liste.
 * @param widget Le widget qui a émis le signal pour activer la fonction.
 */
void recherche_via_masque(GtkWidget *widget){
    const char *masque  = fenetre_input_masque(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    affiche_liste(recherche_par_masque(masque,true));
    free((void *)masque);
}

/**
 * Cette fonction permet à l'utilisateur de supprimer une adresse IP en spécifiant
 * à la fois l'adresse IP et le masque.
 * @param widget Le widget qui a émis le signal pour activer la fonction.
 */
void suppression_ip(GtkWidget *widget) {
    const char *ip_address = fenetre_input_adresse_ip(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    const char *masque = fenetre_input_masque(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    if (ip_address && masque) {
        affiche_liste(supprimer_ip(ip_address, masque, true));
        free((void *)ip_address);
        free((void *)masque);
    }
}

/**
 * Cette fonction permet à l'utilisateur d'ajouter une adresse IP en spécifiant
 * à la fois l'adresse IP et le masque.
 * @param widget Le widget qui a émis le signal pour activer la fonction.
 */
void ajout_ip(GtkWidget *widget) {
    const char *ip_address = fenetre_input_adresse_ip(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    const char *masque = fenetre_input_masque(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    affiche_liste(ajouter_ip(ip_address, masque, true));
    free((void *)ip_address);
    free((void *)masque);
}
/**
 * Cette fonction affiche les résultats de la recherche ou de l'opération sur une liste d'adresses IP.
 * @param resultats Les résultats à afficher sous forme de chaîne de caractères.
 */
void affiche_liste(char *resultats) {
    GtkWidget *window, *label;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    window = gtk_dialog_new_with_buttons("Liste IP", NULL, flags,"OK", GTK_RESPONSE_ACCEPT, NULL); 
    label = gtk_label_new(resultats);
    g_free(resultats);  

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(window))), label);
    gtk_widget_show_all(window);

    gint response = gtk_dialog_run(GTK_DIALOG(window));
    if (response == GTK_RESPONSE_ACCEPT) {
        gtk_widget_destroy(window);
    }
}

/**
 * Cette fonction est appelée lorsqu'un bouton de la souris est cliqué dans la zone d'affichage.
 * Elle gère les actions à effectuer en fonction des coordonnées du clic de souris.
 * @param widget Le widget sur lequel le clic de souris a eu lieu.
 * @param event Les informations sur l'événement de souris, notamment les coordonnées du clic.
 * @param data Les données associées à l'événement (non utilisées dans cette fonction).
 */
void clique(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    if (event->type == GDK_BUTTON_PRESS) {
        gint x, y;
        gtk_widget_translate_coordinates(widget, GTK_WIDGET(gtk_widget_get_toplevel(widget)), event->x, event->y, &x, &y);
        printf("%d %d\n",x,y);
        if ((x >= 187 && x <= 480) && (y >= 221 && y <= 314)) {
                ajout_ip(widget);
            }     
        if ((x >= 537 && x <= 826) && (y >= 221 && y <= 314)) {
            suppression_ip(widget);
        }
        if ((x >= 187 && x <= 480) && (y >= 410 && y <= 502)) {
            affiche_liste(lister_ip(true));
        }
        if ((x >= 537 && x <= 826) && (y >= 410 && y <= 502)) {
            recherche_via_masque(widget);
        }
    }
}

/**
 * Cette fonction crée un bouton rectangulaire dans un conteneur fixe avec des paramètres de position, de taille et de texte.
 * Elle connecte également le signal "button-press-event" du bouton à la fonction de gestion de clics.
 * @param fixed Le conteneur fixe dans lequel le bouton sera placé.
 * @param x La coordonnée x du coin supérieur gauche du bouton.
 * @param y La coordonnée y du coin supérieur gauche du bouton.
 * @param l La largeur du bouton.
 * @param h La hauteur du bouton.
 * @param text Le texte à afficher sur le bouton.
 */
void creation_rectangle(GtkWidget *fixed, int x, int y, int l, int h, const gchar *text) {
    GtkWidget *button = gtk_button_new_with_label(text);
    gtk_fixed_put(GTK_FIXED(fixed), button, x, y);
    gtk_widget_set_size_request(button, l, h);
    g_signal_connect(button, "button-press-event", G_CALLBACK(clique), NULL);
}

/**
 * Cette fonction initialise l'interface graphique GTK et crée un menu principal pour l'application IP C-atalogue.
 * Elle affiche des boutons rectangulaires pour ajouter, lister, supprimer des adresses IP et rechercher via le masque.
 * Elle gère également les événements de fermeture de la fenêtre.
 * @param argc Le nombre d'arguments de la ligne de commande.
 * @param argv Un tableau d'arguments de la ligne de commande.
 * @return int Le code de retour de la fonction.
 */
int menu_interface(int argc, char *argv[]) {
    char ***argvc = &argv;
    gtk_init(&argc, argvc);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "IP C-atalogue");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 600);

    fixed = gtk_fixed_new();
    
    gtk_container_add(GTK_CONTAINER(window), fixed);
    creation_rectangle(GTK_WIDGET(fixed), 150, 150, 300, 100, "Ajouter une adresse IP");
    creation_rectangle(GTK_WIDGET(fixed), 150, 339, 300, 100, "Liste les adresses IP");
    creation_rectangle(GTK_WIDGET(fixed), 500, 150, 300, 100, "Supprimer une adresse IP");
    creation_rectangle(GTK_WIDGET(fixed), 500, 339, 300, 100, "Recherche via le masque");
    label = gtk_label_new("IP C-atalogue");

    PangoAttrList *attrList = pango_attr_list_new();
    PangoAttribute *attr = pango_attr_font_desc_new(pango_font_description_from_string("Thema 30"));

    pango_attr_list_insert(attrList, attr);

    gtk_label_set_attributes(GTK_LABEL(label), attrList);
    gtk_fixed_put(GTK_FIXED(fixed), label, 350, 25);
    gtk_widget_show_all(window);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();

    return 0;
}