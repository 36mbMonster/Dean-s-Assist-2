#include <gtk/gtk.h>

GtkBuilder *builder;
GObject *window;

int main(int argc, char *argv[])
{

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "main_win.glade", NULL);

	window = gtk_builder_get_object(builder,"window1");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "Test");

    

    gtk_widget_show(window);
    gtk_main();

    return 0;
}
