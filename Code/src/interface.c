/**
 * @file
 * @brief Ce fichier contient les fonctions pour gérer l'interface graphique.
 */

#include "../includes/catalogue_ip.h"

GtkWidget *fenetre = NULL;
GtkWidget *fixe = NULL;
GtkWidget *label = NULL;
GtkWidget *dialogue = NULL;
GtkWidget *contenu = NULL;
GtkWidget *entrer = NULL;

int global_argc;
char **global_argv;

/**
 * Cette fonction crée une fenêtre de dialogue pour permettre à l'utilisateur
 * de saisir une adresse IP.
 * @param widget Le widget qui a émis le signal pour l'activation de la fonction.
 * @param donnee Les données associées au signal (unused).
 * @return const char* L'adresse IP saisie par l'utilisateur, ou NULL si l'utilisateur a annulé.
 */
const char *fenetre_adresse_ip(GtkWidget *widget, const char *donnee) {
    const gchar *bouton_ecrire_texte = (const gchar *)donnee;
    bouton_ecrire_texte = "Ajouter une adresse IP";
    if (strcmp(bouton_ecrire_texte, "Ajouter une adresse IP") == 0) {
        dialogue = gtk_dialog_new_with_buttons("Ajouter une adresse IP",
        GTK_WINDOW(fenetre), GTK_DIALOG_MODAL, "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_CANCEL, NULL);
        contenu = gtk_dialog_get_content_area(GTK_DIALOG(dialogue));
        entrer = gtk_entry_new();
        gtk_container_add(GTK_CONTAINER(contenu), entrer);
        gtk_widget_show_all(dialogue);
        gint response = gtk_dialog_run(GTK_DIALOG(dialogue));
        if (response == GTK_RESPONSE_ACCEPT) {

            const char *adresse_ip = strdup(gtk_entry_get_text(GTK_ENTRY(entrer)));
            
            g_print("Adresse IP saisie : %s\n", adresse_ip);
            gtk_widget_destroy(dialogue);
            return adresse_ip;
        }
        else if (response == GTK_RESPONSE_CANCEL) {
            gtk_widget_destroy(dialogue);
            return NULL;
        }
        gtk_widget_destroy(dialogue);
        
    }
    return NULL; 
}

/**
 * Cette fonction crée une fenêtre de dialogue permettant à l'utilisateur de saisir un masque.
 * @param widget Le widget qui a émis le signal pour activer la fonction.
 * @param donnee Les données associées au signal (non utilisées).
 * @return const char* Le masque saisi par l'utilisateur, ou NULL si l'utilisateur a annulé.
 */
const char *fenetre_masque(GtkWidget *widget, const char *donnee) {
    const gchar *bouton_ecrire_texte = (const gchar *)donnee;
    bouton_ecrire_texte = "Ajouter un masque";
    if (strcmp(bouton_ecrire_texte, "Ajouter un masque") == 0) {
        dialogue = gtk_dialog_new_with_buttons("Ajouter un masque",
            GTK_WINDOW(fenetre), GTK_DIALOG_MODAL, "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_CANCEL, NULL);
        contenu = gtk_dialog_get_content_area(GTK_DIALOG(dialogue));
        entrer = gtk_entry_new();
        gtk_container_add(GTK_CONTAINER(contenu), entrer);
        gtk_widget_show_all(dialogue);
        gint response = gtk_dialog_run(GTK_DIALOG(dialogue));
        if (response == GTK_RESPONSE_ACCEPT) {

            const char *masque = strdup(gtk_entry_get_text(GTK_ENTRY(entrer)));
            
            g_print("Masque saisie : %s\n", masque);
            gtk_widget_destroy(dialogue);
            return masque;
        }
        else if (response == GTK_RESPONSE_CANCEL) {
            gtk_widget_destroy(dialogue);
            return NULL;
        }
        gtk_widget_destroy(dialogue);
    }
    return NULL; 
}




/**
 * Cette fonction affiche en forme de Popup les messages d'erreur ou liste.
 * Tout affichage n'ayant pas besoin de rentrer une valeur.
 * @param resultats Les résultats à afficher sous forme de chaîne de caractères.
 */
void affiche_popup(char *resultats) {
    GtkWidget *fenetre, *label;
    GtkDialogFlags drapeau = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    fenetre = gtk_dialog_new_with_buttons("IP C-atalogue", NULL, drapeau,"OK", GTK_RESPONSE_ACCEPT, NULL); 
    label = gtk_label_new(resultats);
    g_free(resultats);  

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(fenetre))), label);
    gtk_widget_show_all(fenetre);

    gint response = gtk_dialog_run(GTK_DIALOG(fenetre));
    if (response == GTK_RESPONSE_ACCEPT) {
        gtk_widget_destroy(fenetre);
    }
}

/**
 * Cette fonction est appelée lorsqu'un utilisateur souhaite ajouter une nouvelle adresse IP à la base de données.
 * Elle récupère l'adresse IP et le masque de sous-réseau saisis par l'utilisateur à partir des fenêtres de saisie.
 * 
 * @param widget Le widget déclencheur de l'événement, le bouton dans l'interface graphique.
 */
void ajout_ip(GtkWidget *widget) {
    const char *adresse_ip = fenetre_adresse_ip(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    const char *masque = fenetre_masque(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    g_print("ip: %s, mask: %s\n", adresse_ip, masque);
    affiche_popup(ajouter_ip(adresse_ip, masque, true));
    free((void *)adresse_ip);
    free((void *)masque);
}

/**
 * Cette fonction est appelée lorsque l'utilisateur souhaite filtrer les adresses IP de la liste selon un masque spécifié.
 * Elle récupère l'adresse IP et le masque de sous-réseau saisis par l'utilisateur à partir des fenêtres de saisie.
 *
 * @param widget Le widget déclencheur de l'événement, le bouton dans l'interface graphique.
 */
void filtrer_par_mask(GtkWidget *widget) {
    const char *ip  = fenetre_adresse_ip(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    const char *masque  = fenetre_masque(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    affiche_popup(filtered_mask(true, ip, masque));
    free((void *)ip);
    free((void *)masque);
}

/**
 * Cette fonction est appelée lorsque l'utilisateur souhaite supprimer une adresse IP de la base de données.
 * Elle récupère l'adresse IP et le masque de sous-réseau saisis par l'utilisateur à partir des fenêtres de saisie.
 *
 * @param widget Le widget déclencheur de l'événement, le bouton dans l'interface graphique.
 */
void sup_ip(GtkWidget *widget) {
    const char *ip  = fenetre_adresse_ip(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    const char *masque  = fenetre_masque(widget, (gpointer)gtk_button_get_label(GTK_BUTTON(widget)));
    affiche_popup(supprimer_ip(ip, masque, true));
    free((void *)ip);
    free((void *)masque);
}

/**
 * Cette fonction est appelée lorsqu'un événement de clic de souris se produit sur le widget spécifié.
 *
 * @param widget Le widget sur lequel l'événement de clic de souris s'est produit.
 * @param evenement L'événement de bouton de la souris associé.
 * @param donnee Des données supplémentaires associées à l'événement, si nécessaire.
 */
void clique(GtkWidget *widget, GdkEventButton *evenement, gpointer donnee) {
    if (evenement->type == GDK_BUTTON_PRESS) {
        gint x, y;
        gtk_widget_translate_coordinates(widget, GTK_WIDGET(gtk_widget_get_toplevel(widget)), evenement->x, evenement->y, &x, &y);
        if ((x >= 187 && x <= 480) && (y >= 221 && y <= 314)) {
            ajout_ip(widget);
        }     
        if ((x >= 537 && x <= 826) && (y >= 221 && y <= 314)) {
            sup_ip(widget);
        }
        if ((x >= 187 && x <= 480) && (y >= 410 && y <= 502)) {
            affiche_popup(lister_ip(true));
        }
        if ((x >= 537 && x <= 826) && (y >= 410 && y <= 502)) {
            filtrer_par_mask(widget);
        }
    }
}

/**
 * Cette fonction crée un bouton rectangulaire dans un conteneur fixe avec les spécifications fournies.
 *
 * @param fixe Le conteneur fixe (fixe) auquel le bouton rectangulaire sera attaché.
 * @param x La position horizontale (abscisse) du coin supérieur gauche du bouton dans le conteneur fixe.
 * @param y La position verticale (ordonnée) du coin supérieur gauche du bouton dans le conteneur fixe.
 * @param l La largeur du bouton.
 * @param h La hauteur du bouton.
 * @param text Le texte à afficher sur le bouton.
 */
void creation_rectangle(GtkWidget *fixe, int x, int y, int l, int h, const gchar *text) {
    GtkWidget *button = gtk_button_new_with_label(text);
    gtk_fixed_put(GTK_FIXED(fixe), button, x, y);
    gtk_widget_set_size_request(button, l, h);
    g_signal_connect(button, "button-press-event", G_CALLBACK(clique), NULL);
}
/**
 * Cette fonction initialise l'interface graphique pour afficher un menu principal.
 *
 * @return La fonction retourne 0 une fois que l'interface graphique a été créée et que la boucle est terminée.
 */
int menu_interface() {

    gtk_init(&global_argc, &global_argv);

    fenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(fenetre), "IP C-atalogue");
    gtk_window_set_position(GTK_WINDOW(fenetre), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(fenetre), 10);
    gtk_window_set_default_size(GTK_WINDOW(fenetre), 1000, 600);

    fixe = gtk_fixed_new();
    
    gtk_container_add(GTK_CONTAINER(fenetre), fixe);
    creation_rectangle(GTK_WIDGET(fixe), 150, 150, 300, 100, "Ajouter une adresse IP");
    creation_rectangle(GTK_WIDGET(fixe), 150, 339, 300, 100, "Liste les adresses IP");
    creation_rectangle(GTK_WIDGET(fixe), 500, 150, 300, 100, "Supprimer une adresse IP");
    creation_rectangle(GTK_WIDGET(fixe), 500, 339, 300, 100, "Recherche via le masque");
    label = gtk_label_new("IP C-atalogue");

    PangoAttrList *attrList = pango_attr_list_new();
    PangoAttribute *attr = pango_attr_font_desc_new(pango_font_description_from_string("Thema 30"));

    pango_attr_list_insert(attrList, attr);

    gtk_label_set_attributes(GTK_LABEL(label), attrList);
    gtk_fixed_put(GTK_FIXED(fixe), label, 350, 25);
    gtk_widget_show_all(fenetre);
    g_signal_connect(fenetre, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();

    return 0;
}