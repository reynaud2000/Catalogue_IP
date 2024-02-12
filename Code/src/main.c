#include "../includes/catalogue_ip.h"

int main(int argc, char *argv[]){
    // menu(argc,argv);
    // creer_base_sql();
    listAdr myLst = NULL;
    Loop_Menu(myLst);
    // load_Sql_In_List(myLst);
    freeList(myLst);


    return 0;
}