#include <gtk/gtk.h>
#include <pango/pangocairo.h>
#include <stdio.h>

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *label;


void clique(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    if (event->type == GDK_BUTTON_PRESS) {
        // Convertir les coordonnées de la fenêtre en coordonnées locales du bouton
        gint x, y;
        gtk_widget_translate_coordinates(widget, GTK_WIDGET(gtk_widget_get_toplevel(widget)), event->x, event->y, &x, &y);

        g_print("Clic de souris détecté à la position (%d, %d)\n", x, y);

        if ((x >= 187 && x <= 480) && (y >= 221 && y <= 314)) {
            printf("Clic sur le bouton 'Ajouter une adresse IP'\n");
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
        // Ajoutez des conditions pour les autres boutons si nécessaire
    }
}



void creation_rectangle(GtkWidget *fixed, int x, int y, int width, int height, const gchar *text) {
    GtkWidget *button = gtk_button_new_with_label(text);
    gtk_fixed_put(GTK_FIXED(fixed), button, x, y);
    gtk_widget_set_size_request(button, width, height);
    g_signal_connect(button, "button-press-event", G_CALLBACK(clique), NULL);
}

void menu(int argc, char *argv[]) {
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
    creation_rectangle(GTK_WIDGET(fixed), 150, 339, 300, 100, "Utiliser les adresses dans la base");
    creation_rectangle(GTK_WIDGET(fixed), 500, 150, 300, 100, "Sélectionner une adresse");
    creation_rectangle(GTK_WIDGET(fixed), 500, 339, 300, 100, "Recherche par masque");

    // Utilisez la variable globale label
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

    gtk_widget_show_all(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_main();
}
int main(int argc, char *argv[]) {
    menu(argc, argv);
    return 0;
}
