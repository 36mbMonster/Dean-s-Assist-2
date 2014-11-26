#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.c"
#include "backend.c"

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
void delete_row();
void new_row();
void cell_edited();
void save_as();
void change_digits( GtkWidget *widget, GtkSpinButton *spin );
void generate_sections();
void change_digits();
void write_to_db();

int i = 0;

//GTK Components
GtkBuilder *builder;
GtkWidget *window;
GtkWidget *show_hide_columns_window;
GtkWidget *set_days_window;
GtkFileChooserDialog *file_dialog;
GtkAboutDialog *about_dialog;
GtkMessageDialog *error_dialog;
GtkMessageDialog *generate_sections_dialog;

GtkEntry *save_entry;

GtkButton *error_okay_button;
GtkButton *days_okay_button;
GtkButton *sect_gen_okay_button;
GtkButton *file_chooser_okay;
GtkButton *file_chooser_cancel;
GtkSpinButton *spin_button;

GtkAdjustment *spin_adjust;

GtkWidget *treeview;
GtkTreeModel *model;
GtkTreePath *path;
GtkTreeIter iter;
GtkTreeViewColumn *columns[9];
GtkListStore *store;
GtkTreeSelection *selector;

int days[5];

//Check boxes
GtkCheckButton *monday;
GtkCheckButton *tuesday;
GtkCheckButton *wednesdy;
GtkCheckButton *thursday;
GtkCheckButton *friday;

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

GObject *about_item;
GObject *quit_item;
GObject *show_hide_columns_item;
GObject *new_course_item;
GObject *delete_row_item;
GObject *save_semester_as_item;
GObject *generate_sections_item;

enum
{
	COL_DEPT,
	COL_NUMBER,
	COL_START,
	COL_END,
	COL_DAYS,
	COL_SECT,
	COL_BLDG,
	COL_ROOM,
	COL_INSTR
};