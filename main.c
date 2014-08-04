#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
//function declarations
void show_about_dialog();

int i = 0;

GtkBuilder *builder;
GtkWidget *window;
GtkAboutDialog *about_dialog;

GtkWidget *treeview;
GtkCellRenderer *renderer;
GtkTreeViewColumn *columns[9];
GtkListStore *store;

GObject *about_item;

int main(int argc, char *argv[])
{

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "main_win.glade", NULL);

	//Load windows
	window = GTK_WIDGET(gtk_builder_get_object(builder,"window"));
	about_dialog = GTK_ABOUT_DIALOG(gtk_builder_get_object(builder,"aboutdialog"));

	//Load menu items
	about_item = gtk_builder_get_object(builder, "about");

	//Load other stuff
	//treeview = gtk_builder_get_object(builder, "treeview");
	treeview = gtk_tree_view_new();

	//TEST TEST TEST TEST
	renderer = gtk_cell_renderer_text_new();

	for(i = 1; i < 10; i++)
	{
		char test[20] = "treeviewcolumn";
		char buffer[2];
		sprintf(buffer, "%d", i);
		strcat(test,buffer);

		columns[i-1] = gtk_builder_get_object(builder, test);
	}

	//column = gtk_tree_view_column_new_with_attributes("List Items",renderer, "text", 0, NULL);
	//gtk_tree_view_column_set_visible (columns[0],FALSE);
  	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), columns[4]);

  	//store = gtk_list_store_new(1, G_TYPE_STRING);

  	//gtk_tree_view_set_model(GTK_TREE_VIEW(treeview),
	//GTK_TREE_MODEL(store));

	//Connect signals
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(about_item, "activate", G_CALLBACK(show_about_dialog), NULL);

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
