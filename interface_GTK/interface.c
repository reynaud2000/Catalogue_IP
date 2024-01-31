// Importer des bibliothèques
//gcc -o interface.o interface.c $(pkg-config --cflags --libs gtk+-3.0)
#include <gtk/gtk.h>
#include <pango/pangocairo.h>
#include <stdio.h>
#include "interface.h"
#include "../projet/catalogue_ip.h"

//déclarer les variable globales.

GtkWidget *window = NULL;
GtkWidget *fixed = NULL;
GtkWidget *label = NULL;
GtkWidget *dialog = NULL;
GtkWidget *content_area = NULL;
GtkWidget *entry = NULL;

char fenetre_input(GtkWidget *widget, gpointer data) {

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
            const char *ip_address = gtk_entry_get_text(GTK_ENTRY(entry));
            g_print("Adresse IP saisie : %s\n", ip_address);
            gtk_widget_destroy(dialog);
            return *ip_address;
        }

        // Fermer la popup
        gtk_widget_destroy(dialog);
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

        // Vérifie si le boutton Ajouter une adresse est cliqué

        if ((x >= 187 && x <= 480) && (y >= 221 && y <= 314)) {

            // appelle la fonction fenetre_input pour ouvrir une popup permmettant à l'utilisateur d'écrire.

            fenetre_input(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget))); 
        }
        if ((x >= 537 && x <= 826) && (y >= 221 && y <= 314)) {
            printf("Clic sur le bouton 'Selectionner une adresse'\n");
        }
        if ((x >= 187 && x <= 480) && (y >= 410 && y <= 502)) {
            printf("Clic sur le bouton 'Utiliser les adresses dans la base'\n");
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

void menu_interface(int argc, char *argv[]) {

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
    creation_rectangle(GTK_WIDGET(fixed), 150, 339, 300, 100, "Utiliser les adresses dans la base");
    creation_rectangle(GTK_WIDGET(fixed), 500, 150, 300, 100, "Sélectionner une adresse");
    creation_rectangle(GTK_WIDGET(fixed), 500, 339, 300, 100, "Recherche par masque");

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

    //ance la boucle principale de l'interface graphique GTK
    gtk_main();
}

