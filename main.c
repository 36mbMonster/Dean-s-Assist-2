#include <stdio.h>
#include <stdlib.h>

#include "iface.h"
#include "pairs.h"


int main()
{
    //init();
    init("test", main_menu_labels);
    show_main_menu();

    return 0;
}
