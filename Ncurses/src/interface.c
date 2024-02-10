#include "../includes/my.h"

GtkWidget *window = NULL;
GtkWidget *fixed = NULL;
GtkWidget *label = NULL;
GtkWidget *dialog = NULL;
GtkWidget *content_area = NULL;
GtkWidget *entry = NULL;

int global_argc;
char **global_argv;


bool validData_interface(char *ip, char *mask) {
    if (ip == NULL) {
        printf("\033[1;31mVous devez entrer une IP et un MASQUE.\033[0m\n");
        return false;
    }
    else {
        int Ip1, Ip2, Ip3, Ip4;
        int mask1, mask2, mask3, mask4;
        sscanf(ip, "%d.%d.%d.%d", &Ip1, &Ip2, &Ip3, &Ip4);
        sscanf(mask, "%d.%d.%d.%d", &mask1, &mask2, &mask3, &mask4);
        if (verifyNumberIP(Ip1) && verifyNumberIP(Ip2) && 
        verifyNumberIP(Ip3) && verifyNumberIP(Ip4) && 
        verifyNumberMask(mask1) && verifyNumberMask(mask2) && 
        verifyNumberMask(mask3) && verifyNumberMask(mask4))
            return true;
        else
            return false;
    }
}

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
 * Liste les adresses IP et leurs informations depuis la base de données.
 * @param graphique Indique si l'affichage est destiné à une interface graphique.
 * @return Une chaîne de caractères contenant la liste des adresses IP et leurs informations.
 *         En cas d'erreur, renvoie une chaîne vide ou un message d'erreur.
 *         La mémoire allouée doit être libérée après utilisation.
 */
char *lister_ip(bool graphique) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    char *resultats = g_strdup("");

    rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        resultats = g_strdup("Impossible d'ouvrir la base de données");
        return resultats;
    }

    const char *sql = "SELECT * FROM ip_masque";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        resultats = g_strdup("Erreur lors de la préparation de la requête");
        return resultats;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *ip = sqlite3_column_text(stmt, 1);
        const unsigned char *masque = sqlite3_column_text(stmt, 2);
        const unsigned char *ip_binaire = sqlite3_column_text(stmt, 3);
        const unsigned char *ip_hex = sqlite3_column_text(stmt, 4);
        if(graphique) {
            resultats = g_strconcat(resultats, "Liste des adresses IP :\n", "------------------------------------------------\n",
            "IP: ", ip, " | Masque: ", masque, "| Binaire:", ip_binaire, "| Hexa:", ip_hex, "\n", NULL);
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return resultats;
}

char *filtered_mask(bool graphique, const char *ip_add, const char *mask) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    char *resultats = g_strdup("");

    if (validData_interface((char *) ip_add, (char *)mask) == false) {
        resultats = g_strdup("les données sont incorrect");
        return resultats;
    }

    rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        resultats = g_strdup("Impossible d'ouvrir la base de données");
        return resultats;
    }

    const char *sql = "SELECT * FROM ip_masque";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        resultats = g_strdup("Erreur lors de la préparation de la requête");
        return resultats;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *ip = sqlite3_column_text(stmt, 1);
        const unsigned char *masque = sqlite3_column_text(stmt, 2);
        const unsigned char *ip_binaire = sqlite3_column_text(stmt, 3);
        const unsigned char *ip_hex = sqlite3_column_text(stmt, 4);
        if(graphique) {
            if (strcmp(getIpNetwork((char *)ip, (char *)mask), getIpNetwork((char *)ip_add, (char *)mask)) == 0) {
                resultats = g_strconcat(resultats, "Liste des adresses IP :\n", "------------------------------------------------\n",
                "IP: ", ip, " | Masque: ", masque, "| Binaire:", ip_binaire, "| Hexa:", ip_hex, "\n", NULL);
            }
        }
       
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return resultats;
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

void ajout_ip(GtkWidget *widget) {
    const char *ip_address = fenetre_input_adresse_ip(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    const char *masque = fenetre_input_masque(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    g_print("ip: %s, mask: %s\n", ip_address, masque);
    affiche_liste(ajouter_ip(ip_address, masque, true));
    free((void *)ip_address);
    free((void *)masque);
}

void filtrer_par_mask(GtkWidget *widget) {
    const char *ip  = fenetre_input_adresse_ip(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    const char *masque  = fenetre_input_masque(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    affiche_liste(filtered_mask(true, ip, masque));
    free((void *)ip);
    free((void *)masque);
}

void delete_ip(GtkWidget *widget) {
    const char *ip  = fenetre_input_adresse_ip(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    const char *masque  = fenetre_input_masque(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    affiche_liste(supprimer_ip(ip, masque, true));
    free((void *)ip);
    free((void *)masque);
}


void clique(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    if (event->type == GDK_BUTTON_PRESS) {
        gint x, y;
        gtk_widget_translate_coordinates(widget, GTK_WIDGET(gtk_widget_get_toplevel(widget)), event->x, event->y, &x, &y);
        g_print("x= %d;  y= %d\n", x, y);
        if ((x >= 187 && x <= 480) && (y >= 221 && y <= 314)) {
            ajout_ip(widget);
        }     
        if ((x >= 537 && x <= 826) && (y >= 221 && y <= 314)) {
            delete_ip(widget);
        }
        if ((x >= 187 && x <= 480) && (y >= 410 && y <= 502)) {
            affiche_liste(lister_ip(true));
        }
        if ((x >= 537 && x <= 826) && (y >= 410 && y <= 502)) {
            filtrer_par_mask(widget);
        }
    }
}

void creation_rectangle(GtkWidget *fixed, int x, int y, int l, int h, const gchar *text) {
    GtkWidget *button = gtk_button_new_with_label(text);
    gtk_fixed_put(GTK_FIXED(fixed), button, x, y);
    gtk_widget_set_size_request(button, l, h);
    g_signal_connect(button, "button-press-event", G_CALLBACK(clique), NULL);
}

int menu_interface() {

    gtk_init(&global_argc, &global_argv);

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