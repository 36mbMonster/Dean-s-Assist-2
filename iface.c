#include "iface.h"

/*
char *options[] =
{
    "Enter data for class schedules",
    "Enter faculty data", "Reports menu",
    "Create new semester",
    "About",
    " ",
    "Exit"
};*/
char **options;
char * title;
int row, col, wrow, wcol;
WINDOW *win;
DMENU this_menu;

/*void init()
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
}*/

//void init(char *a_title, char **a_options)
void init(DMENU menu)
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

    this_menu = menu;
    options = menu.options;
    title =  menu.label;

}

void show_main_menu()
{


    ITEM **menu_items;
    ITEM *current;
    MENU *main_menu;

    int num_choices;
    int i,loop = 1;

    num_choices = ARRAY_SIZE(options);
    menu_items = (ITEM **)calloc(num_choices + 1, sizeof(ITEM *));
    main_menu = new_menu((ITEM **)menu_items);

    for(i = 0; i < num_choices; i++)
        menu_items[i] = new_item(options[i], "");
    menu_items[num_choices] = (ITEM*)NULL;
    main_menu = new_menu((ITEM **)menu_items);

    //Bind menu to window
    set_menu_win(main_menu, win);
    //derwin(number_of_lines, chars_wide, y, x)
    set_menu_sub(main_menu, derwin(win, 7, 32, wrow/4, wcol/4));

    //Set marker
    set_menu_mark(main_menu, "> ");

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
    post_menu(main_menu);
    wrefresh(win);

    int item_index = 0;
    while(loop == 1)
    {

        switch(getch())
        {
            case KEY_DOWN:
                menu_driver(main_menu, REQ_DOWN_ITEM);
                item_index++;
                break;
            case KEY_UP:
                menu_driver(main_menu, REQ_UP_ITEM);
                item_index--;
                break;
            case '\n':
                if(this_menu.type == MENU_IFACE)
                {
                    init(this_menu.next_menus[item_index]);
                    show_main_menu();
                }

                if(this_menu.type == CLASS_FORM_IFACE)
                {
                    free_dean_menu(main_menu, menu_items, num_choices);
                    init_form(this_menu.next_menus[item_index]);
                    show_form();
                }

                if(!strcmp(item_name(current_item(main_menu)), "Exit") || !strcmp(item_name(current_item(main_menu)), "Back"))
                    loop = 0;
                break;
        }
        wrefresh(win);
    }
    free_dean_menu(main_menu, menu_items, num_choices);
    endwin();
}

void free_dean_menu(MENU *main_menu, ITEM **menu_items, int num_choices)
{
    unpost_menu(main_menu);
    free_menu(main_menu);
    int i = 0;
    for(i = 0; i < num_choices; i++)
            free_item(menu_items[i]);
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
