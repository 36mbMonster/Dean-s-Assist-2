#include <gtk/gtk.h>

void init_print();
void start_printer(GtkWidget *window);
void begin_print();
void draw_page();

GtkPrintSettings *print_settings;

void start_printer(GtkWidget *window)
{
	GtkPrintOperation *print_operation;

	print_operation = gtk_print_operation_new();

	if(print_settings != NULL)
		gtk_print_operation_set_print_settings(print_operation, print_settings);

	g_signal_connect(print_operation, "begin_print", G_CALLBACK(begin_print),NULL);
	g_signal_connect(print_operation, "draw_page", G_CALLBACK(draw_page),NULL);

	int res;
	res = gtk_print_operation_run (print_operation, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG,GTK_WINDOW (window), NULL);

	if(res == GTK_PRINT_OPERATION_RESULT_APPLY)
	{
		printf("placeholder");
	}
}

void begin_print()
{

}

void draw_page()
{

}
