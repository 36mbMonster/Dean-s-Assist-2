#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.c"
#include "backend.c"
#include "print.c"

//Function declarations

//show dialog functions
void show_about_dialog();
void show_adjust_columns_dialog();
void show_generate_sections();

//hid dialog functions
void hide_adjust_columns_dialog();
void hide_set_days_dialog();
void hide_error_dialog();
void hide_file_dialog();

//Read or write data
void check_clicked();
void time_gen();
void delete_row();
void new_row();
void cell_edited();
void new_semester();
void save_as();
void load_file();
void generate_sections();
void file_dialog_okay();
void write_to_db();
void read_from_db();
void prep_printer();

int i = 0;
int file_mode;

//GTK Components
GtkBuilder *builder;
GtkWidget *window;
GtkWidget *show_hide_columns_window;
GtkWidget *set_days_window;
GtkFileChooserDialog *file_dialog;
GtkAboutDialog *about_dialog;
GtkMessageDialog *error_dialog;
GtkMessageDialog *generate_sections_dialog;

//Buttons
GtkButton *error_okay_button;
GtkButton *days_okay_button;
GtkButton *sect_gen_okay_button;
GtkSpinButton *spin_button;

GtkAdjustment *spin_adjust;

GtkTreeView *treeview;
GtkTreeModel *model;
GtkTreePath *path;
GtkTreeIter iter;
GtkTreeViewColumn *columns[9];
GtkListStore *store;
GtkTreeSelection *selector;

int days[5];
gboolean gen_times;

//Check boxes
GtkCheckButton *monday;
GtkCheckButton *tuesday;
GtkCheckButton *wednesdy;
GtkCheckButton *thursday;
GtkCheckButton *friday;

GtkCheckButton *time_offset;

//Cell text renderers
GtkCellRendererText *dept_text;
GtkCellRendererText *number_text;
GtkCellRendererText *start_text;
GtkCellRendererText *end_text;
GtkCellRendererText *days_text;
GtkCellRendererText *sect_text;
GtkCellRendererText *bldg_text;
GtkCellRendererText *room_text;
GtkCellRendererText *instructor_text;

//Menu items
GObject *about_item;
GObject *quit_item;
GObject *show_hide_columns_item;
GObject *new_course_item;
GObject *delete_row_item;
GObject *save_semester_item;
GObject *save_semester_as_item;
GObject *load_semester_item;
GObject *generate_sections_item;
GObject *print_item;
GObject *new_semester_item;
