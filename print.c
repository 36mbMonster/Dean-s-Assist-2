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

int school_year;
char *school_season;

char *content[MAX_LINES]; //content in lines

void start_printer();
void begin_print();
void draw_page();
void load_data();

char *format_ident();
char *format_line();

GtkPrintSettings *print_settings;
GtkTreeModel *model;
GtkListStore *store;

void start_printer(GtkWidget *window, GtkTreeModel *amodel, GtkListStore *astore)
{
	GError *error;
	GtkPrintOperation *print_operation;
	print_settings = NULL;
	print_operation = gtk_print_operation_new();
	model = amodel;
	store = astore;

	if(print_settings != NULL)
		gtk_print_operation_set_print_settings(print_operation, print_settings);

	g_signal_connect(print_operation, "begin-print", G_CALLBACK(begin_print),NULL);
	g_signal_connect(print_operation, "draw-page", G_CALLBACK(draw_page),NULL);

	gtk_print_operation_set_use_full_page (print_operation, FALSE);
	gtk_print_operation_set_unit (print_operation, GTK_UNIT_POINTS);
	gtk_print_operation_set_embed_page_setup (print_operation, TRUE);

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

	//Calculate the number of lines and load the semester data into an array.
	load_data();

	pages = (lines - 1) / lines_per_page + 1;

	printf("# of pages: (%d - 1)/%d + 1 = %d\n",lines,lines_per_page,pages);

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

	//Get the print context and calculate the width based on it.
	cr = gtk_print_context_get_cairo_context(context);
	width = gtk_print_context_get_width(context);

	//Set the font type, size and scale.
	layout = gtk_print_context_create_pango_layout (context);
	font = pango_font_description_from_string ("courier");

	pango_font_description_set_size(font, FONT_SIZE * PANGO_SCALE);
	pango_layout_set_font_description (layout, font);
	pango_font_description_free (font);
	pango_layout_set_width(layout, -1);
	pango_layout_get_size (layout, NULL, &layout_height);

	/**
	***************************************************************************
	*							  Print the Header						  	  *
	***************************************************************************
	*/

	struct tm *local;
	char str_date[10];
	char text[25];

	//Calculate the local time
	time_t t;
	t = time(NULL);
	local = localtime(&t);
	//No carriage return character here because it was causing garbage characters to be printed.
	strftime(str_date, sizeof(str_date), "%m/%d/%y\n",local);

	//Print the number of pages
	sprintf(text,"Page No. %d\r\n",page + 1);
	strcat(text, str_date);
	pango_layout_set_text(layout, text, -1);
	pango_layout_get_pixel_size (layout, &text_width, &text_height);
	cairo_move_to(cr, 0, current_y);
	pango_cairo_show_layout(cr, layout);
	printf("%s\n",text);

	//Print the first part of the organizatino header
    pango_layout_set_text(layout, "San Jose State University", -1);
    pango_layout_get_pixel_size (layout, &text_width, &text_height);

    if (text_width > width)
	{
		pango_layout_set_width (layout, width);
		pango_layout_set_ellipsize (layout, PANGO_ELLIPSIZE_START);
		pango_layout_get_pixel_size (layout, &text_width, &text_height);
	}

	//Print the text 2 lines below the top.
	current_y = FONT_SIZE * 2;
    cairo_move_to(cr, (width - text_width)/2, current_y);
    pango_cairo_show_layout(cr, layout);

	//Print the rest of the organization header one line below that.
    pango_layout_set_text(layout, "School of Business", -1);
    pango_layout_get_pixel_size (layout, &text_width, &text_height);
	current_y += FONT_SIZE;
    cairo_move_to(cr, (width - text_width)/2, current_y);
    pango_cairo_show_layout(cr, layout);

	//Print the semester and year.
	char semester_header[15];
	sprintf(semester_header, "%s %d\r\n",school_season, school_year);
	pango_layout_set_text(layout, semester_header, -1);
	pango_layout_get_pixel_size(layout, &text_width, &text_height);
	cairo_move_to(cr, (width - text_width), 0);
	pango_cairo_show_layout(cr, layout);



	/**
	***************************************************************************
	*							  Print the Data						  	  *
	***************************************************************************
	*/


	int line = page * lines_per_page;


	//Print the column headers a lot farther down after the page header.
	current_y += GAP;
	cairo_move_to(cr, 0, current_y);
	char column_text[100];
	//sprintf(column_text, "%-5s%-6s%-13s%-12s%-4s%-5s%-5s%-20s\n","Dep","CN","Time", "Day(s)", "Sec", "Bldg", "Room", "Instructor");
	sprintf(column_text, "\t\t%-13s%-8s%-4s%-5s%-5s%-20s\r\n","Time", "Day(s)", "Sec", "Bldg", "Room", "Instructor");
	pango_layout_set_text(layout,column_text,-1);
	pango_cairo_show_layout(cr, layout);

	//Start printing the data with a one row gap beneath the column headers.
	current_y += FONT_SIZE;
	cairo_move_to(cr, 0, current_y);

	//print here
	line = page * lines_per_page;
	int i;
	for(i = 0; i < lines_per_page && line < lines; i++)
	{
		pango_layout_set_text(layout, content[line], -1);
		pango_cairo_show_layout(cr, layout);
		cairo_rel_move_to (cr, 0, FONT_SIZE + 3);
		line++;
	}

    g_object_unref(layout);
}

void load_data()
{
	lines = 0;

	gboolean more_list;
	GtkTreeIter iter;
	more_list = gtk_tree_model_get_iter_first(model, &iter);

	char *dept, *num, *days, *bldg, *instr;
	int start, end, sect, room;

	char *previous_cn;
	char *prime_col;
	previous_cn = malloc(sizeof(char)*4);
	previous_cn = "";
	int ident_size = 0;
	int col_id;

	//Load the data from the model into the string buffer.
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

		char *text; //row
		//char ident[12];
		char *ident;
		gtk_tree_sortable_get_sort_column_id(GTK_TREE_SORTABLE(store), &col_id, NULL); //You need to know the prime column
		//printouts need to be relative to the prime column.

		//FIX THIS NEXT!!!
		if(strcmp(previous_cn,num) != 0)
		{
			//sprintf(ident,"**%-5s%-5s",dept,num);
			ident = format_ident(dept, num, start, end, days, sect, bldg, room, instr);
			ident_size = strlen(ident);

			content[lines] = malloc(sizeof(char*)*ident_size);
			strcpy(content[lines],ident);
			printf("%s\n",content[lines]);
            /*pango_layout_set_text(layout, ident, -1);
            cairo_rel_move_to (cr, 0, FONT_SIZE);
            pango_cairo_show_layout(cr, layout);

            printf("%s\n",ident);*/
            lines++;
		}

			strncpy(ident," ",ident_size);
			int k;
			for(k = 0; k < ident_size-1; k++)
				strcat(ident," ");

			text = format_line(dept, num, start, end, days, sect, bldg, room, instr, ident);
            //sprintf(text,"%s%-6d%s%-6d%-12s%-4d%-5s%-5d%-20s\r\n",ident,start,"-",end,days,sect,bldg,room,instr);
            //sprintf(text,"%s%-5s%-6s%-6d%s%-6d%-12s%-4d%-5s%-5d%-20s\n",ident,dept,num,start,"-",end,days,sect,bldg,room,instr);
            /*pango_layout_set_text(layout, text, -1);
            cairo_rel_move_to (cr, 0, FONT_SIZE);
            pango_cairo_show_layout(cr, layout);*/

            more_list = gtk_tree_model_iter_next(model, &iter);
            previous_cn = num;

            content[lines] = malloc(sizeof(char*)*150);
            strcpy(content[lines],text);
            printf("%s\n",content[lines]);
            lines++;

	}

	free(previous_cn);

}

char *format_ident(char *dept, char *num, int start, int end,
                   char *days, int sect, char *bldg, int room,
                   char *instr)
{
	char *ident = malloc(15*sizeof(char));
	int col_id;

	gtk_tree_sortable_get_sort_column_id(GTK_TREE_SORTABLE(store), &col_id, NULL);

	switch(col_id)
	{
		case COL_DEPT:
		case COL_NUMBER:
			sprintf(ident,"**%-5s%-5s",dept,num);
			break;
		case COL_START:
			sprintf(ident,"**%-5d-%-5d",start,end);
			break;
		case COL_END:
			sprintf(ident,"**%-5d-%-5d",end,start);
			break;
		case COL_DAYS:
			sprintf(ident,"**%-5s",days);
			break;
		case COL_SECT:
			sprintf(ident,"**%-5d",sect);
			break;
		case COL_BLDG:
		case COL_ROOM:
			sprintf(ident,"**%-5s%-5d",bldg,room);
			break;
		case COL_INSTR:
			sprintf(ident,"**%-5s",instr);
			break;
	}

	return ident;
}

char *format_line(char *dept, char *num, int start, int end,
                   char *days, int sect, char *bldg, int room,
                   char *instr, char *ident)
{
	char *text = malloc(150*sizeof(char));
	int col_id;

	gtk_tree_sortable_get_sort_column_id(GTK_TREE_SORTABLE(store), &col_id, NULL);

	switch(col_id)
	{
		case COL_DEPT:
		case COL_NUMBER:
			sprintf(text,"%s%-6d%s%-6d%-12s%-4d%-5s%-5d%-20s\r\n",ident,start,"-",end,days,sect,bldg,room,instr);
			break;
		case COL_START:
		case COL_END:
			sprintf(text,"%s%-6s%-6s%-12s%-4d%-5s%-5d%-20s\n",ident,dept,num,days,sect,bldg,room,instr);
			break;
		case COL_DAYS:
			sprintf(text,"%s%-6d%s%-6d%-12s%-4d%-5s%-5d%-20s\r\n",ident,start,"-",end,days,sect,bldg,room,instr);
			break;
		case COL_SECT:
			sprintf(text,"%s%-6d%s%-6d%-12s%-4d%-5s%-5d%-20s\r\n",ident,start,"-",end,days,sect,bldg,room,instr);
			break;
		case COL_BLDG:
		case COL_ROOM:
			sprintf(text,"%s%-6d%s%-6d%-12s%-4d%-5s%-5d%-20s\r\n",ident,start,"-",end,days,sect,bldg,room,instr);
			break;
		case COL_INSTR:
			sprintf(text,"%s%-6d%s%-6d%-12s%-4d%-5s%-5d%-20s\r\n",ident,start,"-",end,days,sect,bldg,room,instr);
			break;
	}

	return text;
}
