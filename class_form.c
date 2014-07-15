#include "iface.h"
#include "class_form.h"
#include "util.h"


char * title;
int row, col, wrow, wcol;
WINDOW *win;
DMENU this_form_menu;
FIELD *field[3];
FORM  *this_form;

void init_form(DMENU menu)
{
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, row, col);
    init_pair(1, COLOR_BLUE, COLOR_BLACK);

    win = newwin(row-5, col-5, 4, 4);
    getmaxyx(stdscr, wrow, wcol);
    keypad(win, TRUE);

    field[0] = new_field(1, 10, 4, 18, 0, 0);
	field[1] = new_field(1, 10, 6, 18, 0, 0);
	field[2] = NULL;

    set_field_back(field[0], A_UNDERLINE);
	field_opts_off(field[0], O_AUTOSKIP);
    set_field_back(field[1], A_UNDERLINE);
	field_opts_off(field[1], O_AUTOSKIP);

}

void show_form()
{

    this_form = new_form(field);
    post_form(this_form);
    mvprintw(4, 10, "Value 1:");
	mvprintw(6, 10, "Value 2:");
    refresh();

    //Window border
    box(win, 0, 0);

    //Draw title box & title
    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, 1, ((col-strlen(title))/2)-1 , title);
    wattroff(win, COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, col-7);
	mvwaddch(win, 2, col-6, ACS_RTEE);
    refresh();


    refresh();
    wrefresh(win);

    endwin();
}


//Thanks to  Pradeep Padala
//http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters
                                 * for the vertical and horizontal
                                 * lines        */

	return local_win;
}
