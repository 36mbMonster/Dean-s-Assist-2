#include <stdio.h>
#include <stdlib.h>

#include "iface.h"
#include "pairs.h"

int main()
{
    DMENU next_from_main[] = {class_menu, fac_menu, reports_menu};
    DMENU main_menu = {"Dean's Assist 2", main_menu_labels, next_from_main};
    //init();
    //init("Dean's Assist 2", main_menu_labels);
    init(main_menu);
    show_main_menu();

    return 0;
}
