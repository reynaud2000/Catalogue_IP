#include "../includes/my.h"

char *ip_to_binary(char *ip) {
    char result[40];
    int Ip1, Ip2, Ip3, Ip4;
    sscanf(ip, "%d.%d.%d.%d", &Ip1, &Ip2, &Ip3, &Ip4);
    snprintf(result, sizeof(result), "%08b.%08b.%08b.%08b\t", Ip1, Ip2, Ip3, Ip4);
    return strdup(result);
}

// void ip_to_binary_ncurses(char *ip, WINDOW *win, int x, int y) {
//     int Ip1, Ip2, Ip3, Ip4;
//     sscanf(ip, "%d.%d.%d.%d", &Ip1, &Ip2, &Ip3, &Ip4);
//     mvwprintw(win, x, y, "- Binaire: %08b.%08b.%08b.%08b", Ip1, Ip2, Ip3,Ip4);
// }

char* ip_to_hex(char *ip) {
    char result[16];
    int Ip1, Ip2, Ip3, Ip4;
    sscanf(ip, "%d.%d.%d.%d", &Ip1, &Ip2, &Ip3, &Ip4);
    snprintf(result, sizeof(result), "%X.%X.%X.%X\n", Ip1, Ip2, Ip3,Ip4);
    return strdup(result);
}

// void ip_to_hex_ncurses(char *ip, WINDOW *win, int x, int y) {
//     int Ip1, Ip2, Ip3, Ip4;
//     sscanf(ip, "%d.%d.%d.%d", &Ip1, &Ip2, &Ip3, &Ip4);
//     mvwprintw(win, x, y, "- Hexadécimale: %X.%X.%X.%X", Ip1, Ip2, Ip3,Ip4);
// }