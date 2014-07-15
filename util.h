#pragma once

#define DMENU  struct deans_menu

struct deans_menu
{
    char *label;
    char **options;
    DMENU *next_menus;
};
