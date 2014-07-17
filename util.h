#pragma once

#define DMENU  struct deans_menu

const int MENU_IFACE;
const int CLASS_FORM_IFACE;

struct deans_menu
{
    char *label;
    int type;
    char **options;
    DMENU *next_menus;
};
