#pragma once

#include <ncurses.h>
#include <menu.h>
#include <string.h>

#include "util.h"

#define ARRAY_SIZE(a) (sizeof(a) )
#define PAIR  struct label_func_pair

void init();
//void init(char *title, char **items);
void init(DMENU menu);
void show_main_menu();

struct label_func_pair
{
    char *label;
    void (*func) (char *, char **);
};
