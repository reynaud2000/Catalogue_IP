
/**
 * @file
 * @brief Ce fichier contient les fonctions utils qui aide aux autres fonctions.
 */
#include "../includes/catalogue_ip.h"

/**
 * Cette fonction lit une ligne depuis l'entrée standard (stdin) et la stocke dans un tampon de caractères.
 *
 * @return La ligne lue depuis stdin (sans le caractère de saut de ligne à la fin), ou NULL en cas d'erreur.
 */
char *get(void)
{
    char *tab = NULL;
    size_t len = 0;
    int n = getline(&tab, &len, stdin);

    if (n == -1)
        return (NULL);
    tab[n - 1] = '\0';
    return (tab);
}
/**
 * Cette fonction prend une adresse IP et un masque sous-réseau au format décimal
 * et retourne l'adresse réseau correspondante.
 *
 * @param ip   L'adresse IP au format décimal pointé.
 * @param mask Le masque de sous-réseau au format décimal pointé.
 * @return     L'adresse réseau résultante au format décimal pointé.
 */
char *getIpNetwork(char *ip, char *mask) {
    int Ip1, Ip2, Ip3, Ip4;
    int mask1, mask2, mask3, mask4;
    char IpNetwork[16];

    sscanf(ip, "%d.%d.%d.%d", &Ip1, &Ip2, &Ip3, &Ip4);
    sscanf(mask, "%d.%d.%d.%d", &mask1, &mask2, &mask3, &mask4);
    sprintf (IpNetwork, "%d.%d.%d.%d", Ip1 & mask1, Ip2 & mask2, Ip3 & mask3, Ip4 & mask4);
    return (strdup(IpNetwork));
}
/**
 * Cette fonction calcule la longueur d'une chaîne de caractères.
 *
 * @param str La chaîne de caractères dont on veut calculer la longueur.
 * @return    La longueur de la chaîne de caractères.
 */
int my_strlen(char const *str)
{
    int p;
    for (p = 0; str[p] != '\0'; p++);
    return (p);
}

void free_2d_array(char **tab)
{
    for (int i = 0; tab[i]!= NULL; i++)
        free(tab[i]);
}

/**
 * Compte le nombre de mots dans une chaîne de caractères.
 *
 * @param str La chaîne de caractères à analyser.
 * @return Le nombre de mots dans la chaîne.
 */
int number(char const *str)
{
    int a = 0;

    for (int i = 0; str[i]; i++)
        if (str[i] == ' ')
            a++;
    return (a + 1);
}

/**
 * Convertit une chaîne de caractères en tableau de mots.
 *
 * @param str La chaîne de caractères à convertir.
 * @return Un tableau de mots.
 */
char **my_str_to_word_array(char const *str)
{
    int comp = 0, p = 0;
    int a = 0, i = 0, b = number(str);
    char **buffer = malloc (sizeof(char *) * (number(str) + 2));

    for (a = 0; a - 1 != my_strlen(str); a++, p++) {
        for (; str[a] != ' ' && a < my_strlen(str); a++)
            comp++;
        buffer[p] = malloc (sizeof(char) * comp + 1);
        comp = 0;
    }
    p = 0;
    comp = 0;
    for (a = 0; i - 1 != my_strlen(str); buffer[a][p] = '\0', a++, p++, i++)
        for (p = 0; str[i] != ' ' && i < my_strlen(str); p++, i++)
            buffer[a][p] = str[i];
    buffer[a] = NULL;
    return (buffer);
}

/**
 * Vérifie si un nombre donné est une valeur valide pour une adresse IP.
 *
 * @param number Le nombre à vérifier.
 * @return true si le nombre est valide pour une adresse IP, sinon false.
 */
bool verifyNumberIP(int number) {
    return true ? number >= 0 && number <= 255 : false;
}
/**
 * Vérifie si un nombre donné est une valeur valide pour un masque de sous-réseau.
 *
 * @param number Le nombre à vérifier.
 * @return true si le nombre est valide pour un masque de sous-réseau, sinon false.
 */
bool verifyNumberMask(int number) {
    if (number == 0 || number == 128 || number == 192 || number == 224 || number == 240 || number == 248 || number == 252 || number == 254 || number == 255)
        return true;
    else
        return false;
}
/**
 * Vérifie si les données IP et masque fournies sont valides.
 *
 * @param ip L'adresse IP à vérifier.
 * @param mask Le masque de sous-réseau à vérifier.
 * @param line La ligne saisie par l'utilisateur.
 * @return true si les données IP et masque sont valides, sinon false.
 */
bool validData(char *ip, char *mask, char *line) {
    if (ip == NULL || number(line) != 3) {
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
 * Vérifie si les données IP et masque fournies sont valides dans l'interface ncurses.
 *
 * @param ip L'adresse IP à vérifier.
 * @param mask Le masque de sous-réseau à vérifier.
 * @param line La ligne saisie par l'utilisateur.
 * @param win La fenêtre ncurses où afficher les messages d'erreur.
 * @return true si les données IP et masque sont valides, sinon false.
 */
bool validData_ncurses(char *ip, char *mask, char *line, WINDOW *win) {
    if (ip == NULL || number(line) != 3) {
        TextColored(win, 5, 48, "Vous devez entrer une IP et un MASQUE.", 1);
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
 * Filtre les adresses IP dans la base de données en fonction de l'adresse IP et du masque spécifiés.
 *
 * @param graphique Indique si l'interface graphique est utilisée.
 * @param ip_add L'adresse IP à utiliser pour le filtrage.
 * @param mask Le masque de sous-réseau à utiliser pour le filtrage.
 * @return Une chaîne de caractères contenant les résultats du filtrage.
 *         Si aucune adresse IP n'est trouvée, une chaîne vide est retournée.
 *         En cas d'erreur, un message d'erreur approprié est retourné.
 */
char *filtered_mask(bool graphique, const char *ip_add, const char *mask) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    char *resultats = g_strdup("");


    if (validData_i((char *) ip_add, (char *) mask) == false) {
        resultats = g_strdup("Merci de rentrer un masque ou une ip valide en X.X.X.X");
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
 * Vérifie si l'adresse IP et le masque spécifiés sont valides.
 *
 * @param ip L'adresse IP à vérifier.
 * @param mask Le masque de sous-réseau à vérifier.
 * @return true si l'adresse IP et le masque sont valides, sinon false.
 *         Affiche un message d'erreur approprié s'ils ne sont pas valides.
 */
bool validData_i(char *ip, char *mask) {
    if (ip == NULL || mask == NULL) {
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