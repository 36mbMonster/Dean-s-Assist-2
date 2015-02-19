#include <gtk/gtk.h>

void init_print();
void start_printer(GtkWidget *window);
void begin_print();
void draw_page();

GtkPrintSettings *print_settings;

void start_printer(GtkWidget *window)
{
	GError *error;
	GtkPrintOperation *print_operation;
	print_settings = NULL;

	print_operation = gtk_print_operation_new();

	if(print_settings != NULL)
		gtk_print_operation_set_print_settings(print_operation, print_settings);

	g_signal_connect(print_operation, "begin-print", G_CALLBACK(begin_print),NULL);
	g_signal_connect(print_operation, "draw-page", G_CALLBACK(draw_page),NULL);

	int res;
	res = gtk_print_operation_run (print_operation, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG,GTK_WINDOW (window), &error);

	if(res == GTK_PRINT_OPERATION_RESULT_ERROR)
	{
		GtkWidget *error_dialog;
		error_dialog = gtk_message_dialog_new(GTK_WINDOW (window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "The following error occured:\n%s",error->message);
	}

	else if(res == GTK_PRINT_OPERATION_RESULT_APPLY)
	{
		if(print_settings != NULL)
			g_object_unref(print_settings);
		print_settings =  g_object_ref(gtk_print_operation_get_print_settings(print_operation));
	}
	else
	{
		printf("wrong signal: %d\n",res);
	}

	g_object_unref(print_operation);
}

void begin_print()
{

}

void draw_page()
{

}
