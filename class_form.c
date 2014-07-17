#include "class_form.h"

char * title;
int row, col, wrow, wcol;
WINDOW *win;
DMENU this_form_menu;
FIELD *field[3];
FORM  *this_form;

void init_form(DMENU menu)
{
    initscr();
    //start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, row, col);
    //init_pair(1, COLOR_BLUE, COLOR_BLACK);

    scale_form(this_form, &row, &col);
    win = newwin(row-5, col-5, 4, 4);
    getmaxyx(stdscr, wrow, wcol);

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
    keypad(win, TRUE);
    set_form_win(this_form, win);
    set_form_sub(this_form, derwin(win, 7, 32, wrow/4, wcol/4));

    //Window border
    box(win, 0, 0);

    //Draw title box & title
    //wattron(win, COLOR_PAIR(1));
    //mvwprintw(win, 1, ((col-strlen(title))/2)-1 , title);
    //wattroff(win, COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, col-7);
	mvwaddch(win, 2, col-6, ACS_RTEE);
	refresh();

    post_form(this_form);
    wrefresh(win);

    mvprintw(14, 32, "Value 1:");
	mvprintw(16, 32, "Value 2:");
    //refresh();

    int i, item_index = 0;
    int loop = 1;
    char in;
    while((in = getch()) && loop == 1)
    {

        switch(getch())
        {
            case KEY_DOWN:
                form_driver(this_form, REQ_NEXT_FIELD);
                form_driver(this_form, REQ_END_LINE);
                item_index++;
                break;
            case KEY_UP:
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

        //wrefresh(win);
        //refresh();
    }

    unpost_form(this_form);
    free_form(this_form);
    free_field(field[0]);
    free_field(field[1]);

    endwin();
}
