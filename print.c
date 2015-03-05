#include <gtk/gtk.h>
#include <time.h>
#include <math.h>
#include "constants.h"

#define GAP (2 * 12 * 72 / 25.4)

const int FONT_SIZE = 12;

int pages;
int lines;
int height;
int lines_per_page;

void start_printer();
void begin_print();
void draw_page();
void end_print();

GtkPrintSettings *print_settings;
GtkTreeModel *model;

void start_printer(GtkWidget *window, GtkTreeModel *amodel)
{
	GError *error;
	GtkPrintOperation *print_operation;
	print_settings = NULL;
	print_operation = gtk_print_operation_new();
	model = amodel;

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

	height = gtk_print_context_get_height(context);
	lines_per_page = floor(height/FONT_SIZE);
	lines = 0;

	//Calculate the number of lines
	gboolean more_list;
	GtkTreeIter iter;
	more_list = gtk_tree_model_get_iter_first(model, &iter);

	while(more_list)
	{
		lines++;
		more_list = gtk_tree_model_iter_next(model, &iter);
	}

	pages = (lines - 1) / lines_per_page + 1;

	gtk_print_operation_set_n_pages(operation,pages);
}

void draw_page(GtkPrintOperation *operation,
			GtkPrintContext *context,
			gint page,
			gpointer user_data)
{
	printf("draw\n");

    int layout_height;
	double width;
	int text_width, text_height;
	int current_y = 0;

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
	font = pango_font_description_from_string ("courier");
	pango_font_description_set_size(font, FONT_SIZE * PANGO_SCALE);
	pango_layout_set_font_description (layout, font);
	pango_font_description_free (font);

	/**
	***************************************************************************
	*							  Print the Header						  	  *
	***************************************************************************
	*/

	pango_layout_set_width(layout, -1);

	//pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);
	time_t t;
	struct tm *local;
	char str_date[10];
	t = time(NULL);
	local = localtime(&t);
	strftime(str_date, sizeof(str_date), "%m/%d/%y\n",local);
	char text[25];
	sprintf(text,"Page No. %d\n",pages);
	strcat(text, str_date);
	pango_layout_set_text(layout, text, -1);
	pango_cairo_show_layout(cr, layout);
	printf("%s\n",text);

	//pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
	//pango_layout_get_pixel_size(layout, &text_width, &text_height);
    pango_layout_set_text(layout, "San Jose State University", -1);
    pango_layout_get_pixel_size (layout, &text_width, &text_height);

    if (text_width > width)
	{
		pango_layout_set_width (layout, width);
		pango_layout_set_ellipsize (layout, PANGO_ELLIPSIZE_START);
		pango_layout_get_pixel_size (layout, &text_width, &text_height);
	}

	current_y = FONT_SIZE * 2;
    cairo_move_to(cr, (width - text_width)/2, current_y);
    pango_cairo_show_layout(cr, layout);

    pango_layout_set_text(layout, "School of Business", -1);
    pango_layout_get_pixel_size (layout, &text_width, &text_height);
	current_y += FONT_SIZE;
    cairo_move_to(cr, (width - text_width)/2, current_y);
    pango_cairo_show_layout(cr, layout);

	pango_layout_set_text(layout, "Fall/Spring <year>\n", -1);
	pango_layout_get_pixel_size(layout, &text_width, &text_height);
	cairo_move_to(cr, (width - text_width), 0);
	pango_cairo_show_layout(cr, layout);



	/**
	***************************************************************************
	*							  Print the Data						  	  *
	***************************************************************************
	*/

	char *dept, *num, *days, *bldg, *instr;
	int start, end, sect, room;
	gboolean more_list;
	GtkTreeIter iter;
	more_list = gtk_tree_model_get_iter_first(model, &iter);

	//pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);
	current_y += GAP;
	cairo_move_to(cr, 0, current_y);
	char column_text[100];
	sprintf(column_text, "%-5s%-6s%-13s%-12s%-4s%-5s%-5s%-20s\n","Dep","CN","Time", "Day(s)", "Sec", "Bldg", "Room", "Instructor");
	pango_layout_set_text(layout,column_text,-1);
	pango_cairo_show_layout(cr, layout);

	current_y += FONT_SIZE;
	cairo_move_to(cr, 0, current_y);
	while(more_list)
	{
		gtk_tree_model_get(model, &iter,
		COL_DEPT, &dept,
		COL_NUMBER, &num,
		COL_START, &start,
		COL_END, &end,
		COL_DAYS, &days,
		COL_SECT, &sect,
		COL_BLDG, &bldg,
		COL_ROOM, &room,
		COL_INSTR, &instr,
		-1);

		char text[150];
		sprintf(text,"%-5s%-6s%-6d%s%-6d%-12s%-4d%-5s%-5d%-20s\n",dept,num,start,"-",end,days,sect,bldg,room,instr);
		pango_layout_set_text(layout, text, -1);
		cairo_rel_move_to (cr, 0, FONT_SIZE);
		pango_cairo_show_layout(cr, layout);

		more_list = gtk_tree_model_iter_next(model, &iter);
		printf("%s\n",text);
	}

    g_object_unref(layout);
}

void end_print()
{
	printf("printing ended.\n");
}
