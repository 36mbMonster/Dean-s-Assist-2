#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.c"
//function declarations
void show_about_dialog();
void show_adjust_columns_dialog();
void hide_adjust_columns_dialog();
void delete_row();
void new_row();
void cell_edited();

int i = 0;

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *show_hide_columns_window;
GtkAboutDialog *about_dialog;

GtkWidget *treeview;
GtkTreeModel *model;
GtkTreeModel *filter;
GtkTreePath *path;
GtkTreeIter iter;
GtkTreeViewColumn *columns[9];
GtkListStore *store;
GtkTreeSelection *selector;

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
	about_dialog = GTK_ABOUT_DIALOG(gtk_builder_get_object(builder,"aboutdialog"));

	//Load menu items
	about_item = gtk_builder_get_object(builder, "about");
	quit_item = gtk_builder_get_object(builder, "quit");
	show_hide_columns_item = gtk_builder_get_object(builder, "show_hide_columns");
	new_course_item = gtk_builder_get_object(builder, "new_course");
	delete_row_item = gtk_builder_get_object(builder, "delete");

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

	//Connect signals
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(about_item, "activate", G_CALLBACK(show_about_dialog), NULL);
	g_signal_connect(quit_item, "activate", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(new_course_item, "activate", G_CALLBACK(new_row), NULL);
	g_signal_connect(show_hide_columns_item, "activate", G_CALLBACK(show_adjust_columns_dialog), NULL);
	g_signal_connect(delete_row_item, "activate", G_CALLBACK(delete_row), NULL);
	g_signal_connect(show_hide_columns_window, "hide", G_CALLBACK(hide_adjust_columns_dialog), NULL);


	//Connect column signals.
	g_signal_connect(dept_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(number_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(start_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(end_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(days_text, "edited", G_CALLBACK(cell_edited), NULL);
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
		    if(is_int(new_text))
                printf("%s\n","error"); //temporary. Put a message box with an error message here.
			int u = atoi(new_text);
			gtk_list_store_set(store, &iter,
								COL_NUMBER, new_text,
								-1);
		}
		break;
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
