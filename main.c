
#include <stdio.h>
#include <stdlib.h>

#include "iface.h"
#include "pairs.h"

int main()
{
    //This is the next set of submenus that will be rendered after the main menu.
    DMENU class_form = {"Add New Class", CLASS_FORM_IFACE};

    DMENU next_from_class[] = {class_form};
    DMENU class_menu = {"Class Scheduling", MENU_IFACE, class_menu_labels, next_from_class};

    DMENU fac_menu = {"Faculty", MENU_IFACE, fac_menu_labels};

    DMENU reports_menu = {"Reports", MENU_IFACE, reports_menu_labels};

    DMENU next_from_main[] = {class_menu, fac_menu, reports_menu};
    DMENU main_menu = {"Dean's Assist 2", MENU_IFACE, main_menu_labels, next_from_main};
    //init();
    //init("Dean's Assist 2", main_menu_labels);
    //init(main_menu);
    //show_main_menu();
    init_form(class_form);
    show_form();

    return 0;
}
