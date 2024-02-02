// Importer des bibliothèques

//gcc -o interface.o interface.c $(pkg-config --cflags --libs gtk+-3.0) -pthread
#include <gtk/gtk.h>
#include <pango/pangocairo.h>
#include <stdio.h>
#include <pthread.h>
#include "interface.h"
#include "../project_with_sql/catalogue_ip.h"


//déclarer les variable globales.

GtkWidget *window = NULL;
GtkWidget *fixed = NULL;
GtkWidget *label = NULL;
GtkWidget *dialog = NULL;
GtkWidget *content_area = NULL;
GtkWidget *entry = NULL;

const char *fenetre_input_adresse_ip(GtkWidget *widget, const char *data) {

    /*
     Cette fonction un char, elle permet d'ouvrir une popup permettant d'écrire une adresse IP et de la valider.
     elle prend en paramètre  des données et un widget.
    */
    const gchar *bouton_ecrire_texte = (const gchar *)data;

    if (strcmp(bouton_ecrire_texte, "Ajouter une adresse IP") == 0) {

        // créer la popup pour écrire unea dresse ip

        dialog = gtk_dialog_new_with_buttons("Ajouter une adresse IP",
            GTK_WINDOW(window), GTK_DIALOG_MODAL, "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_CANCEL, NULL);

        // récupère la zone du contenu de dialog

        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

        // Ajouter un champ de texte 
        entry = gtk_entry_new();
        gtk_container_add(GTK_CONTAINER(content_area), entry);

        // Affiche tout les Widget

        gtk_widget_show_all(dialog);

        gint response = gtk_dialog_run(GTK_DIALOG(dialog));

        if (response == GTK_RESPONSE_ACCEPT) {

            // Récupérer la valeur écrite 
            const char *ip_address = strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
            
            g_print("Adresse IP saisie : %s\n", ip_address);
            gtk_widget_destroy(dialog);
            return ip_address;
        }
        /*else if (response == GTK_RESPONSE_CANCEL)
        {
            gtk_widget_destroy(dialog);
        }*/
        
        // Fermer la popup
        gtk_widget_destroy(dialog);
        
    }
    return NULL; 
}

const char *fenetre_input_masque(GtkWidget *widget, const char *data) {

    /*
     Cette fonction un char, elle permet d'ouvrir une popup permettant d'écrire une adresse IP et de la valider.
     elle prend en paramètre  des données et un widget.
    */
    const gchar *bouton_ecrire_texte = (const gchar *)data;
    bouton_ecrire_texte = "Ajouter un masque";
    if (strcmp(bouton_ecrire_texte, "Ajouter un masque") == 0) {

        // créer la popup pour écrire un masque

        dialog = gtk_dialog_new_with_buttons("Ajouter un masque",
            GTK_WINDOW(window), GTK_DIALOG_MODAL, "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_CANCEL, NULL);

        // récupère la zone du contenu de dialog

        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

        // Ajouter un champ de texte 
        entry = gtk_entry_new();
        gtk_container_add(GTK_CONTAINER(content_area), entry);

        // Affiche tout les Widget

        gtk_widget_show_all(dialog);

        gint response = gtk_dialog_run(GTK_DIALOG(dialog));

        if (response == GTK_RESPONSE_ACCEPT) {

            // Récupérer la valeur écrite 
            const char *masque = strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
            
            g_print("Masque saisie : %s\n", masque);
            gtk_widget_destroy(dialog);
            return masque;
        }

        // Fermer la popup
        gtk_widget_destroy(dialog);
    }
    return NULL; 
}

void popup_erreur(){
    
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons("Attention", NULL, flags,
                                     "OK", GTK_RESPONSE_ACCEPT,NULL);
    GtkWidget *label = gtk_label_new("Erreur de la saisi de l'adresse IP ou masque. Merci de le saisir sous forme X.X.X.X");
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), label);

    // Affiche tout
    gtk_widget_show_all(dialog);

    // Traitement de la réponse
     gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {

            gtk_widget_destroy(dialog);
    }

}

void affiche_liste_ip() {
    GtkWidget *window, *label;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    window = gtk_dialog_new_with_buttons("Liste IP", NULL, flags, NULL);

    char *resultats = lister_ip(true);  
    label = gtk_label_new(resultats);
    g_free(resultats);  

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(window))), label);
    gtk_widget_show_all(window);

    gint response = gtk_dialog_run(GTK_DIALOG(window));
    if (response == GTK_RESPONSE_ACCEPT) {
        gtk_widget_destroy(window);
    }
}


void clique(GtkWidget *widget, GdkEventButton *event, gpointer data) {

    /*
        Cette fonction ne renvois rien et  gère la gestion du clique avec la souris. 
        elle prend en paramètre un widget, des données, et un évenement.
    */

    // Vérifie si le boutton est pressé
    if (event->type == GDK_BUTTON_PRESS) {

        // Convertir les coordonnées en coordonées graphiques (repère othorgonal)

        gint x, y;
        gtk_widget_translate_coordinates(widget, GTK_WIDGET(gtk_widget_get_toplevel(widget)), event->x, event->y, &x, &y);
        printf("%d %d\n",x,y);
        // Vérifie si le boutton Ajouter une adresse est cliqué

        if ((x >= 187 && x <= 480) && (y >= 221 && y <= 314)) {
            int add_ip = 0;
            // appelle la fonction fenetre_input_adresse_ip pour ouvrir une popup permmettant à l'utilisateur d'écrire.
            while (add_ip != 1)
            {
                const char *ip_address = fenetre_input_adresse_ip(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
                const char *masque  = fenetre_input_masque(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
                add_ip = ajouter_ip(ip_address, masque, true);
                if(add_ip != 1){
                    popup_erreur();
                }
                free((void *)ip_address);
                free((void *)masque);
            }
        }
            

        if ((x >= 537 && x <= 826) && (y >= 221 && y <= 314)) {
            printf("Clic sur le bouton 'Supprimer une adresse IP'\n");
        }
        if ((x >= 187 && x <= 480) && (y >= 410 && y <= 502)) {
            printf("Clic sur le bouton 'Liste les adresses IP'\n");
            affiche_liste_ip();
        }
        if ((x >= 537 && x <= 826) && (y >= 410 && y <= 502)) {
            printf("Clic sur le bouton 'Recherche par masque'\n");
        }
    
}

}


void creation_rectangle(GtkWidget *fixed, int x, int y, int l, int h, const gchar *text) {

    /*
        Cette fonction (ne retourne rien) créer un rectangle comme futur bouton cliquable.
        Elle prend en paramètre une attache (fixer la fenetre), des positions x,y et une hauteur et largeur.
        Enfin elle prend en paramètre une chaine de caractère.

        
    */
    // Créer un nouveaux bouton avec un labell
    GtkWidget *button = gtk_button_new_with_label(text);
    // Fixe le bouton à une position 
    gtk_fixed_put(GTK_FIXED(fixed), button, x, y);
    // Dimmensionne la taille du bouton 
    gtk_widget_set_size_request(button, l, h);
    // connecte le bouton avec l'évenement de la fonction clique (gestion des clique de souris)
    g_signal_connect(button, "button-press-event", G_CALLBACK(clique), NULL);
}

int menu_interface(int argc, char *argv[]) {


    /*
        Cette fonction (retourne rien) permet de créer un menu (avec des boutons).
        Elle prend en paramètre le nombre d'arguments passés au programme et le tableau associé.
    */

    //initie la bibliothèque GTK
    char ***argvc = &argv;
    gtk_init(&argc, argvc);

    //Créer une fenetre principale 
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "IP C-atalogue");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    //Dimensionne la taille et les bordure de la fenêtre 
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 600);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    //Création des bouton avec la fonction creation de rectange
    creation_rectangle(GTK_WIDGET(fixed), 150, 150, 300, 100, "Ajouter une adresse IP");
    creation_rectangle(GTK_WIDGET(fixed), 150, 339, 300, 100, "Liste les adresses IP");
    creation_rectangle(GTK_WIDGET(fixed), 500, 150, 300, 100, "Supprimer une adresse IP");
    creation_rectangle(GTK_WIDGET(fixed), 500, 339, 300, 100, "Recherche par masque de sous réseau");

    // Utilise la variable globale label pour nommé la fenêtre 
    label = gtk_label_new("IP C-atalogue");

    // Créez une structure PangoAttrList pour stocker les attributs de la police
    PangoAttrList *attrList = pango_attr_list_new();

    // Créez une structure PangoAttribute pour définir la police et la taille
    PangoAttribute *attr = pango_attr_font_desc_new(pango_font_description_from_string("Thema 30"));

    // Ajoutez l'attribut à la liste d'attributs
    pango_attr_list_insert(attrList, attr);

    // Appliquez la liste d'attributs à l'étiquette
    gtk_label_set_attributes(GTK_LABEL(label), attrList);

    // Placez l'étiquette dans la fenêtre
    gtk_fixed_put(GTK_FIXED(fixed), label, 350, 25);

    //Affiche tout les élements de la fenêtre
    gtk_widget_show_all(window);

    //Connect la croix de la fenêtre pour la fermer
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    //lance la boucle principale de l'interface graphique GTK
    gtk_main();

    return 0;
}


