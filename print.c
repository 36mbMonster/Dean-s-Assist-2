#include <gtk/gtk.h>

void init_print();
void start_printer(GtkWidget *window);
void begin_print();
void draw_page();
void end_print();

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
		g_signal_connect(error_dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
		gtk_widget_show(error_dialog);
		g_error_free(error);
	}

	else if(res == GTK_PRINT_OPERATION_RESULT_APPLY)
	{
		if(print_settings != NULL)
			g_object_unref(print_settings);
		print_settings =  g_object_ref(gtk_print_operation_get_print_settings(print_operation));
	}

	g_object_unref(print_operation);
}

void begin_print(GtkPrintOperation *operation,
               GtkPrintContext   *context,
               gpointer           user_data)
{
	printf("begin\n");

	//Fix this later
	gtk_print_operation_set_n_pages(operation,1);
}

void draw_page(GtkPrintOperation *operation,
			GtkPrintContext *context,
			gint page,
			gpointer user_data)
{
	printf("draw\n");

    int layout_height;
	double width;
	double text_height;

	cairo_t *cr;
	PangoLayout *layout;
	PangoFontDescription *font;

	cr = gtk_print_context_get_cairo_context(context);
	width = gtk_print_context_get_width(context);

	//Draw a rectangle to represent the paper to be printed.
	//cairo_set_source_rgb(cr, 0, 0, 0);
	//cairo_rectangle(cr, 0, 0, width, gtk_print_context_get_height(context)-20);

	//cairo_fill(cr);

	layout = gtk_print_context_create_pango_layout (context);
	font = pango_font_description_from_string ("sans 14");
	pango_layout_set_font_description (layout, font);
	pango_font_description_free (font);

    pango_layout_set_text(layout, "Hello World!", -1);
    pango_layout_set_width(layout, width * PANGO_SCALE);
    pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);

    pango_layout_get_size(layout, NULL, &layout_height);
    //text_height = (double)layout_height / PANGO_SCALE;

    //cairo_move_to(cr, width / 2, (gtk_print_context_get_height(context)-20 - text_height) / 2);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);
}

void end_print()
{
	printf("printing ended.\n");
}
