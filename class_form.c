#include "class_form.h"

#define DOWN_KEY 2
#define UP_KEY 3

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

    scale_form(this_form, &row, &col);
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
    set_form_win(this_form, win);
    set_form_sub(this_form, derwin(win, 7, 32, wrow/4, wcol/4));

    //Window border
    box(win, 0, 0);

    //Draw title box & title
    wattron(win, COLOR_PAIR(1));
    //mvwprintw(win, 1, (int)((col-strlen(title))/2)-1 , title);
    wattroff(win, COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, col-7);
	mvwaddch(win, 2, col-6, ACS_RTEE);
	refresh();

    post_form(this_form);
    //wrefresh(win);

    mvwprintw(win, 10, 30, "Value 1:");
	mvwprintw(win, 12, 30, "Value 2:");
    wrefresh(win);

    int item_index = 0;
    int loop = 1;
    char in;
    while((in = wgetch(win)) != KEY_F(1))
    {

        switch(in)
        {
            case DOWN_KEY:
                form_driver(this_form, REQ_NEXT_FIELD);
                form_driver(this_form, REQ_END_LINE);
                item_index++;
                break;
            case UP_KEY:
                form_driver(this_form, REQ_PREV_FIELD);
                form_driver(this_form, REQ_END_LINE);
                item_index--;
                break;
            case '\n':
                form_driver(this_form, REQ_NEXT_FIELD);
                form_driver(this_form, REQ_END_LINE);
                break;
            case '\t':
                form_driver(this_form, REQ_NEXT_FIELD);
                form_driver(this_form, REQ_END_LINE);
                break;
            default:
                form_driver(this_form, in);
                break;
        }

    }

    unpost_form(this_form);
    free_form(this_form);
    free_field(field[0]);
    free_field(field[1]);

    endwin();
}
