#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *label;
GtkWidget *entry;

void on_button_click(GtkWidget *widget, gpointer data) {
    if (strcmp((char *)data, "Ajouter une adresse IP") == 0) {
        // Créer une boîte de dialogue pour la saisie de l'adresse IP
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Ajouter une adresse IP",
                                                         GTK_WINDOW(window),
                                                         GTK_DIALOG_MODAL,
                                                         "Valider",
                                                         GTK_RESPONSE_ACCEPT,
                                                         "Annuler",
                                                         GTK_RESPONSE_CANCEL,
                                                         NULL);

        GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

        // Ajouter un champ de texte à la boîte de dialogue
        entry = gtk_entry_new();
        gtk_container_add(GTK_CONTAINER(content_area), entry);

        gtk_widget_show_all(dialog);

        gint response = gtk_dialog_run(GTK_DIALOG(dialog));

        if (response == GTK_RESPONSE_ACCEPT) {
            // Récupérer la valeur de l'entrée et faire quelque chose avec elle
            const char *ip_address = gtk_entry_get_text(GTK_ENTRY(entry));
            g_print("Adresse IP saisie : %s\n", ip_address);
        }

        // Fermer la boîte de dialogue
        gtk_widget_destroy(dialog);
    }
}

void create_rectangle(GtkWidget *fixed, int x, int y, int width, int height, const gchar *text) {
    GtkWidget *button = gtk_button_new_with_label(text);
    gtk_fixed_put(GTK_FIXED(fixed), button, x, y);
    gtk_widget_set_size_request(button, width, height);
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_click), (gpointer)text);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "IP C-atalogue");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    create_rectangle(GTK_WIDGET(fixed), 50, 150, 300, 100, "Ajouter une adresse IP");
    create_rectangle(GTK_WIDGET(fixed), 50, 339, 300, 100, "Utiliser les adresses dans la base");
    create_rectangle(GTK_WIDGET(fixed), 450, 150, 300, 100, "Sélectionner une adresse");
    create_rectangle(GTK_WIDGET(fixed), 450, 339, 300, 100, "Recherche par masque");

    label = gtk_label_new("IP C-atalogue");
    gtk_fixed_put(GTK_FIXED(fixed), label, 250, 25);

    gtk_widget_show_all(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_main();

    return 0;
}
