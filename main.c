#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.c"
//function declarations
void show_about_dialog();
void show_adjust_columns_dialog();
void hide_adjust_columns_dialog();
void hide_set_days_dialog();
void hide_error_dialog();
void check_clicked();
void delete_row();
void new_row();
void cell_edited();

int i = 0;

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *show_hide_columns_window;
GtkWidget *set_days_window;
GtkAboutDialog *about_dialog;
GtkMessageDialog *error_dialog;

GtkButton *error_okay_button;
GtkButton *days_okay_button;

GtkWidget *treeview;
GtkTreeModel *model;
GtkTreeModel *filter;
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

int main(int argc, char *argv[])
{

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "main_win.glade", NULL);

/**
***************************************************************************
*							  Initialization						  	  *
***************************************************************************
*/

	//Load windows
	window = GTK_WIDGET(gtk_builder_get_object(builder,"window"));
	show_hide_columns_window = GTK_WIDGET(gtk_builder_get_object(builder,"adjust_columns_win"));
	set_days_window = GTK_WIDGET(gtk_builder_get_object(builder,"set_days_win"));
	about_dialog = GTK_ABOUT_DIALOG(gtk_builder_get_object(builder,"aboutdialog"));
	error_dialog = GTK_MESSAGE_DIALOG(gtk_builder_get_object(builder,"error_dialog"));
	error_okay_button = GTK_BUTTON(gtk_builder_get_object(builder,"error_dialog_okay"));
	days_okay_button = GTK_BUTTON(gtk_builder_get_object(builder,"set_days_okay"));

	//Load menu items
	about_item = gtk_builder_get_object(builder, "about");
	quit_item = gtk_builder_get_object(builder, "quit");
	show_hide_columns_item = gtk_builder_get_object(builder, "show_hide_columns");
	new_course_item = gtk_builder_get_object(builder, "new_course");
	delete_row_item = gtk_builder_get_object(builder, "delete");

    //Load check boxes
    monday = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "mo"));
    tuesday = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "tu"));
    wednesdy = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "we"));
    thursday = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "th"));
    friday = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "fr"));

	//Load tree and list related structures.
	treeview = GTK_WIDGET(gtk_builder_get_object(builder, "treeview"));
	selector = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "treeview-selection"));
	store = GTK_LIST_STORE(gtk_builder_get_object(builder,"liststore"));

	//Set the tree model
	//path = gtk_tree_path_new_from_indices(1,-1);
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));

	//Get the columns from the builder and populate the column array.
	dept_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"dept_text"));
	number_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"number_text"));
	start_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"start_text"));
	end_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"end_text"));
	days_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"days_text"));
	sect_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"sect_text"));
	bldg_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"bldg_text"));
	room_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"room_text"));
	instructor_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"instructor_text"));

	//Load the columns. They are all named "treeviewcolumn" followed by a number as defined in the glade file.
	//This makes it easy to load them all through a loop.
	for(i = 1; i < 10; i++)
	{
		char test[20] = "treeviewcolumn";
		char buffer[2];

		//Append the column's number to the end of the string.
		sprintf(buffer, "%d", i);
		strcat(test,buffer);

		columns[i-1] = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, test));
	}

/**
***************************************************************************
*							  Connect Signals						  	  *
***************************************************************************
*/

	//Connect menu signals
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(about_item, "activate", G_CALLBACK(show_about_dialog), NULL);
	g_signal_connect(quit_item, "activate", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(new_course_item, "activate", G_CALLBACK(new_row), NULL);
	g_signal_connect(show_hide_columns_item, "activate", G_CALLBACK(show_adjust_columns_dialog), NULL);
	g_signal_connect(delete_row_item, "activate", G_CALLBACK(delete_row), NULL);
	g_signal_connect(show_hide_columns_window, "hide", G_CALLBACK(hide_adjust_columns_dialog), NULL);
	g_signal_connect(error_okay_button, "clicked", G_CALLBACK(hide_error_dialog), NULL);
	g_signal_connect(days_okay_button, "clicked", G_CALLBACK(hide_set_days_dialog), NULL);

    //Connect check button signals
    g_signal_connect(monday, "clicked", G_CALLBACK(check_clicked), NULL);
    g_signal_connect(tuesday, "clicked", G_CALLBACK(check_clicked), NULL);
    g_signal_connect(wednesdy, "clicked", G_CALLBACK(check_clicked), NULL);
    g_signal_connect(thursday, "clicked", G_CALLBACK(check_clicked), NULL);
    g_signal_connect(friday, "clicked", G_CALLBACK(check_clicked), NULL);

	//Connect column signals.
	g_signal_connect(dept_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(number_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(start_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(end_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(days_text, "editing-started", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(sect_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(bldg_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(room_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(instructor_text, "edited", G_CALLBACK(cell_edited), NULL);

	//Point columns to cell renderers
	g_object_set_data (G_OBJECT (dept_text), "column", GINT_TO_POINTER (COL_DEPT));
	g_object_set_data (G_OBJECT (number_text), "column", GINT_TO_POINTER (COL_NUMBER));
	g_object_set_data (G_OBJECT (start_text), "column", GINT_TO_POINTER (COL_START));
	g_object_set_data (G_OBJECT (end_text), "column", GINT_TO_POINTER (COL_END));
	g_object_set_data (G_OBJECT (days_text), "column", GINT_TO_POINTER (COL_DAYS));
	g_object_set_data (G_OBJECT (sect_text), "column", GINT_TO_POINTER (COL_SECT));
	g_object_set_data (G_OBJECT (bldg_text), "column", GINT_TO_POINTER (COL_BLDG));
	g_object_set_data (G_OBJECT (room_text), "column", GINT_TO_POINTER (COL_ROOM));
	g_object_set_data (G_OBJECT (instructor_text), "column", GINT_TO_POINTER (COL_INSTR));

	//Show the window and start gtk.
    gtk_widget_show(window);
    gtk_main();


    return 0;
}
/**
***************************************************************************
*							Signal Functions						  	  *
***************************************************************************
*/

void show_about_dialog()
{
	//gtk_widget_show_all( about_dialog );
	gtk_dialog_run(GTK_DIALOG(about_dialog));
	gtk_widget_hide(GTK_WIDGET(about_dialog));
}

void show_adjust_columns_dialog()
{
	gtk_widget_show_all(show_hide_columns_window);
}

void hide_adjust_columns_dialog()
{
	gtk_widget_hide(GTK_WIDGET(show_hide_columns_window));
}

void hide_set_days_dialog()
{
	const char *mo = "Mo";
	const char *tu = "Tu";
	const char *we = "We";
	const char *th = "Th";
	const char *fr = "Fr";

	const char *labels[5] = {mo, tu, we, th, fr};
	char out[11];
	out[0] = '\0';

	for(i = 0; i < 5; i++)
	{
		if(days[i] == 1)
			strcat(out, labels[i]);

	}

	gtk_list_store_set(store, &iter,
                        COL_DAYS, out,
                                    -1);

    gtk_widget_hide(GTK_WIDGET(set_days_window));
}

void hide_error_dialog()
{
	gtk_widget_hide(GTK_WIDGET(error_dialog));
}

void check_clicked(GtkCheckButton *check,
                    const gchar         *path_string,
                    gpointer             data)
{
	path = gtk_tree_path_new_from_string (path_string);
	gtk_tree_model_get_iter (model, &iter, path);

    const char *label = gtk_button_get_label(GTK_BUTTON(check));

	if(strcmp(label,"Monday") == 0)
		days[0] = days[0] ^ 1;

	if(strcmp(label,"Tuesday") == 0)
		days[1] = days[1] ^ 1;

	if(strcmp(label,"Wednesday") == 0)
		days[2] = days[2] ^ 1;

	if(strcmp(label,"Thursday") == 0)
		days[3] = days[3] ^ 1;

	if(strcmp(label,"Friday") == 0)
		days[4] = days[4] ^ 1;

}

//Get user input from the cell and update the row
//WORKS!!
void cell_edited(GtkCellRendererText *renderer,
				const gchar         *path_string,
             	const gchar         *new_text,
             	gpointer             data)
{

	gint column = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (renderer), "column"));
	path = gtk_tree_path_new_from_string (path_string);

	gtk_tree_model_get_iter (model, &iter, path);

	switch(column)
	{
		case COL_DEPT:
		{
			//gint u;
			gchar *old_entry;

			gtk_tree_model_get(model, &iter, column, &old_entry, -1);
			g_free(old_entry);

			//u = gtk_tree_path_get_indices(path)[0];
			//g_free(g_array_index(art));

			gtk_list_store_set(store, &iter,
								COL_DEPT, new_text,
								-1);

		}
		break;
		case COL_NUMBER:
		{
		    if(!is_valid_course_num(new_text))
		    {
		    	gtk_message_dialog_format_secondary_markup(error_dialog, "%s", COURSE_NUMBER_ERROR);
				gtk_dialog_run(GTK_DIALOG(error_dialog));

		    }
		    else
		    {
                gtk_list_store_set(store, &iter,
								COL_NUMBER, new_text,
								-1);
		    }

		}
		break;
		case COL_START:
		{
			if(!is_int(new_text) && new_text < 700 && new_text > 2200)
			{
		    	gtk_message_dialog_format_secondary_markup(error_dialog, "%s", INVALID_TIME_ERROR);
				gtk_dialog_run(GTK_DIALOG(error_dialog));
			}
			else
			{
				gint entry = atoi(new_text);
                gtk_list_store_set(store, &iter,
								COL_START, entry,
								-1);
			}
		}
		break;
		case COL_END:
		{
			if(!is_int(new_text) )
			{
		    	gtk_message_dialog_format_secondary_markup(error_dialog, "%s", INVALID_TIME_ERROR);
				gtk_dialog_run(GTK_DIALOG(error_dialog));
			}
			else
			{
				gint entry = atoi(new_text);
                gtk_list_store_set(store, &iter,
								COL_END, entry,
								-1);
			}
		}
		break;
		case COL_DAYS:
		{
			gtk_widget_show_all(set_days_window);
		}
		break;
		case COL_INSTR:
		{
                gtk_list_store_set(store, &iter,
								COL_INSTR, new_text,
								-1);
		}
	}

	gtk_tree_path_free(path);

}

//Deletes course section from the semester.
//WORKS!!
void delete_row()
{
	gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
}

//Adds a new course or section to the list.
//WORKS!!
void new_row()
{

	gtk_tree_model_get_iter(model, &iter, path);

	gtk_list_store_insert(store, &iter, 0);

	//Set the row. Starts at column number zero
	//Adds two blank strings to prevent program from crashing due to null values.
	gtk_list_store_set(store, &iter,
	COL_DEPT, "",
	COL_NUMBER, "",
	COL_START, 0,
	COL_END, 0,
	COL_DAYS, "",
	COL_SECT, 1,
	COL_BLDG, "bbc",
	COL_ROOM, 0,
	COL_INSTR, "",
	-1);
}
