#pragma once

#include <ncurses.h>
#include <menu.h>
#include <string.h>

#define ARRAY_SIZE(a) (sizeof(a) )
#define PAIR  struct label_func_pair

void init();
void init(char *title, char **items);
void show_main_menu();

struct label_func_pair
{
    char *label;
    //void (*next_pair_array) (PAIR *);
};
