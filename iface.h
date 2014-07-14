#pragma once

#include <ncurses.h>
#include <menu.h>
#include <string.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

void init();
void show_main_menu();
