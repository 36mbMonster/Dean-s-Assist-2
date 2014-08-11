#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

//function declarations
void show_about_dialog();
void show_adjust_columns_dialog();
void hide_adjust_columns_dialog();
void delete_row();
void new_row();
char *get_string_from_form();

int i = 0;

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *show_hide_columns_window;
GtkAboutDialog *about_dialog;

GtkWidget*treeview;
GtkTreeModel *model;
GtkTreeModel *filter;
GtkTreePath *path;
GtkTreeIter iter;
GtkCellRenderer *renderer;
GtkTreeViewColumn *columns[9];
GtkListStore *store;
GtkTreeSelection *selector;

GObject *about_item;
GObject *quit_item;
GObject *show_hide_columns_item;
GObject *new_course_item;
GObject *delete_row_item;

int main(int argc, char *argv[])
{

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "main_win.glade", NULL);

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

	//Load other stuff
	treeview = GTK_WIDGET(gtk_builder_get_object(builder, "treeview"));
	selector = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "treeview-selection"));
	store = GTK_LIST_STORE(gtk_builder_get_object(builder,"liststore"));

	//Set the tree model
	path = gtk_tree_path_new_from_indices(1,-1);
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
	//gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);

	//Get the columns from the builder and populate the column array.
	renderer = gtk_cell_renderer_text_new();

    /*store = gtk_list_store_new( 1, G_TYPE_INT );
    for( i = 1; i < 4; i++ )
    {
        GtkTreeIter parent_iter;

        gtk_list_store_append( store, &parent_iter);
        gtk_list_store_set( store, &parent_iter, 0, i, -1 );

    }

    path = gtk_tree_path_new_from_indices(1,-1);
    filter = gtk_tree_model_filter_new(GTK_TREE_MODEL(store), path);*/

	for(i = 1; i < 10; i++)
	{
		char test[20] = "treeviewcolumn";
		char buffer[2];
		sprintf(buffer, "%d", i);
		strcat(test,buffer);

		columns[i-1] = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, test));
	}

	//column = gtk_tree_view_column_new_with_attributes("List Items",renderer, "text", 0, NULL);
	//gtk_tree_view_column_set_visible (columns[0],FALSE);
  	//gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), columns[4]);

  	//store = gtk_list_store_new(1, G_TYPE_STRING);
  	//gtk_tree_view_set_model(GTK_TREE_VIEW(treeview),GTK_TREE_MODEL(store));

	//Connect signals
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(about_item, "activate", G_CALLBACK(show_about_dialog), NULL);
	g_signal_connect(quit_item, "activate", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(new_course_item, "activate", G_CALLBACK(new_row), NULL);
	g_signal_connect(show_hide_columns_item, "activate", G_CALLBACK(show_adjust_columns_dialog), NULL);
	g_signal_connect(delete_row_item, "activate", G_CALLBACK(delete_row), NULL);
	g_signal_connect(show_hide_columns_window, "hide", G_CALLBACK(hide_adjust_columns_dialog), NULL);


	//Connect column signals
	//for(i = 1; i < 10; i++)
	//{
		g_signal_connect(selector, "changed", G_CALLBACK(get_string_from_form), NULL);
	//}
	//gtk_tree_model_get()

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

char *get_string_from_form()
{
	gchar *data;
	GtkTreeIter iter;
	gtk_tree_model_get_iter(model, &iter, path);
	//gtk_tree_store_append( GTK_TREE_STORE(store), &iter, NULL );

	gtk_tree_model_get(model, &iter, &data);

	printf("%s",data);
	return data;
}

void delete_row()
{
	//GtkTreeIter iter;
	//gtk_tree_model_get_iter(model, &iter, path);

	//gtk_tree_model_row_deleted(model, path);
}

//Adds a new row to the list.
void new_row()
{
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
