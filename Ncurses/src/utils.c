#include "../includes/my.h"

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

char *getIpNetwork(char *ip, char *mask) {
    int Ip1, Ip2, Ip3, Ip4;
    int mask1, mask2, mask3, mask4;
    char IpNetwork[16];

    sscanf(ip, "%d.%d.%d.%d", &Ip1, &Ip2, &Ip3, &Ip4);
    sscanf(mask, "%d.%d.%d.%d", &mask1, &mask2, &mask3, &mask4);
    sprintf (IpNetwork, "%d.%d.%d.%d", Ip1 & mask1, Ip2 & mask2, Ip3 & mask3, Ip4 & mask4);
    return (strdup(IpNetwork));
}

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

int number(char const *str)
{
    int a = 0;

    for (int i = 0; str[i]; i++)
        if (str[i] == ' ')
            a++;
    return (a + 1);
}

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

bool verifyNumberIP(int number) {
    return true ? number >= 0 && number <= 255 : false;
}

bool verifyNumberMask(int number) {
    if (number == 0 || number == 128 || number == 192 || number == 224 || number == 240 || number == 248 || number == 252 || number == 254 || number == 255)
        return true;
    else
        return false;
}

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