#pragma once

#include <ncurses.h>
#include <menu.h>
#include <string.h>

#include "util.h"

#define ARRAY_SIZE(a) (sizeof(a) )

void init();
void init(DMENU menu);
void show_main_menu();
