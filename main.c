#include "deans2.h"

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
	file_dialog = GTK_FILE_CHOOSER_DIALOG(gtk_builder_get_object(builder, "filechooserdialog"));
	about_dialog = GTK_ABOUT_DIALOG(gtk_builder_get_object(builder,"aboutdialog"));
	error_dialog = GTK_MESSAGE_DIALOG(gtk_builder_get_object(builder,"error_dialog"));
	generate_sections_dialog = GTK_MESSAGE_DIALOG(gtk_builder_get_object(builder, "gensect_dialog"));

	save_entry = GTK_ENTRY(gtk_builder_get_object(builder, "save_entry"));

	//Load menu items
	about_item = gtk_builder_get_object(builder, "about");
	quit_item = gtk_builder_get_object(builder, "quit");
	show_hide_columns_item = gtk_builder_get_object(builder, "show_hide_columns");
	new_course_item = gtk_builder_get_object(builder, "new_course");
	delete_row_item = gtk_builder_get_object(builder, "delete");
	save_semester_as_item = gtk_builder_get_object(builder, "save_semester_as");
	load_semester_item = gtk_builder_get_object(builder, "load_semester");
	generate_sections_item = gtk_builder_get_object(builder, "generate_sections");

    //Load check boxes
    monday = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "mo"));
    tuesday = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "tu"));
    wednesdy = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "we"));
    thursday = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "th"));
    friday = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "fr"));

	//Load buttons
	error_okay_button = GTK_BUTTON(gtk_builder_get_object(builder,"error_dialog_okay"));
	days_okay_button = GTK_BUTTON(gtk_builder_get_object(builder,"set_days_okay"));
	sect_gen_okay_button = GTK_BUTTON(gtk_builder_get_object(builder, "sect_gen_okay_button"));
	file_chooser_okay = GTK_BUTTON(gtk_builder_get_object(builder, "file_chooser_okay"));
	file_chooser_cancel = GTK_BUTTON(gtk_builder_get_object(builder, "file_chooser_cancel"));
	spin_button = GTK_SPIN_BUTTON(gtk_builder_get_object(builder,"spinbutton"));

	//Load tree and list related structures.
	treeview = GTK_WIDGET(gtk_builder_get_object(builder, "treeview"));
	selector = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "treeview-selection"));
	store = GTK_LIST_STORE(gtk_builder_get_object(builder,"liststore"));

	//Set the tree model
	//path = gtk_tree_path_new_from_indices(1,-1);
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));

	//Get the renderers from the builder and populate the column array.
	dept_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"dept_text"));
	number_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"number_text"));
	start_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"start_text"));
	end_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"end_text"));
	days_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"days_text"));
	sect_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"sect_text"));
	bldg_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"bldg_text"));
	room_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"room_text"));
	instructor_text = GTK_CELL_RENDERER_TEXT(gtk_builder_get_object(builder,"instructor_text"));

	file_chooser_label = GTK_LABEL(gtk_builder_get_object(builder, "file_chooser_label"));

	GtkTreeSortable *sortable;
	sortable = GTK_TREE_SORTABLE(store);

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
		gtk_tree_sortable_set_sort_func(sortable, i-1, sort_by_num, GINT_TO_POINTER(SORTID_BLDG),NULL);
	}

	spin_adjust = gtk_adjustment_new(0, 0, 15, 1, 2, 0);
	gtk_spin_button_set_adjustment(spin_button, spin_adjust);
	chooser = GTK_FILE_CHOOSER(file_dialog);

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
	g_signal_connect(save_semester_as_item, "activate", G_CALLBACK(save_as), NULL);
	g_signal_connect(load_semester_item, "activate", G_CALLBACK(load_file), NULL);
	g_signal_connect(show_hide_columns_window, "hide", G_CALLBACK(hide_adjust_columns_dialog), NULL);
	g_signal_connect(generate_sections_item, "activate", G_CALLBACK(show_generate_sections), NULL);

	//Connect button signals
	g_signal_connect(error_okay_button, "clicked", G_CALLBACK(hide_error_dialog), NULL);
	g_signal_connect(days_okay_button, "clicked", G_CALLBACK(hide_set_days_dialog), NULL);
	g_signal_connect(sect_gen_okay_button, "clicked", G_CALLBACK(generate_sections), NULL);
	g_signal_connect(file_chooser_okay, "clicked", G_CALLBACK(file_dialog_okay), NULL);
	g_signal_connect(file_chooser_cancel, "clicked", G_CALLBACK(hide_file_dialog), NULL);

    //Connect check box signals
    g_signal_connect(monday, "clicked", G_CALLBACK(check_clicked), NULL);
    g_signal_connect(tuesday, "clicked", G_CALLBACK(check_clicked), NULL);
    g_signal_connect(wednesdy, "clicked", G_CALLBACK(check_clicked), NULL);
    g_signal_connect(thursday, "clicked", G_CALLBACK(check_clicked), NULL);
    g_signal_connect(friday, "clicked", G_CALLBACK(check_clicked), NULL);

	//Connect column signals.
	g_signal_connect(columns[0],"clicked", G_CALLBACK(sort_by_alpha), NULL);
	g_signal_connect(columns[1],"clicked", G_CALLBACK(sort_by_alpha), NULL);
	g_signal_connect(columns[2],"clicked", G_CALLBACK(sort_by_num), NULL);
	g_signal_connect(columns[3],"clicked", G_CALLBACK(sort_by_num), NULL);
	g_signal_connect(columns[4],"clicked", G_CALLBACK(sort_by_alpha), NULL);
	g_signal_connect(columns[5],"clicked", G_CALLBACK(sort_by_num), NULL);
	g_signal_connect(columns[6],"clicked", G_CALLBACK(sort_by_alpha), NULL);
	g_signal_connect(columns[7],"clicked", G_CALLBACK(sort_by_num), NULL);
	g_signal_connect(columns[8],"clicked", G_CALLBACK(sort_by_alpha), NULL);

	//Connect cell renderer signals.
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

void save_as()
{
	file_mode = WRITE;
	gtk_widget_show_all(GTK_WIDGET(file_dialog));
}

void load_file()
{
	file_mode = READ;
	gtk_widget_show_all(GTK_WIDGET(file_dialog));
	gtk_widget_hide(GTK_WIDGET(save_entry));
	gtk_widget_hide(GTK_WIDGET(file_chooser_label));
}

void file_dialog_okay()
{
	if(file_mode == WRITE)
		write_to_db();
	else if(file_mode == READ)
		read_from_db();
}

void show_generate_sections()
{
	gtk_widget_show_all(GTK_WIDGET(generate_sections_dialog));
}

void show_adjust_columns_dialog()
{
	gtk_widget_show_all(show_hide_columns_window);
}

void hide_adjust_columns_dialog()
{
	gtk_widget_hide(GTK_WIDGET(show_hide_columns_window));
}

void write_to_db()
{
	char *db_name = (char *) gtk_entry_get_text(save_entry);
	char *fullname = file_extension_correct(db_name);
	create_db(fullname);
	gboolean more_list = 0;

	char *dept, *num, *days, *bldg, *instr;
	int start, end, sect, room;

	more_list = gtk_tree_model_get_iter_first(model, &iter);

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

		char statement[150];

		//Generate dynamic SQL statement.
		sprintf(statement,"insert into CoB_Sched values(\'%s\',\'%s\',%d,%d,\'%s\',%d,\'%s\',%d,\'%s\');",dept,num,start,end,days,sect,bldg,room,instr);
		printf("%s\n",statement);
		execute_sql(statement);

		//Get the next row
		more_list = gtk_tree_model_iter_next(model, &iter);


	}
	close_db();
	gtk_widget_hide(GTK_WIDGET(file_dialog));
}

void read_from_db()
{
	char *filename = gtk_file_chooser_get_filename(chooser);

	char **dept, **num, **days, **bldg, **instr;
	int *start, *end, *sect, *room;
	int i, size;

	open_db(filename);
	execute_sql("select * from CoB_Sched;");

    dept = get_dept_vals();
    num = get_num_vals();
	days = get_day_vals();
	bldg = get_bldg_vals();
	instr = get_instr_vals();
	start = get_start_vals();
	end = get_end_vals();
	sect = get_sect_vals();
	room = get_room_vals();

	gtk_tree_model_get_iter(model, &iter, path);
	size = get_size();

    for(i = 0; i < size; i++)
    {
        gtk_list_store_insert(store, &iter, 0);

        gtk_list_store_set(store, &iter,
        COL_DEPT, dept[i],
        COL_NUMBER, num[i],
        COL_START, start[i],
        COL_END, end[i],
        COL_DAYS, days[i],
        COL_SECT, sect[i],
        COL_BLDG, bldg[i],
        COL_ROOM, room[i],
        COL_INSTR, instr[i],
        -1);

        gtk_tree_model_iter_next(model, &iter);
    }
    close_db();
    gtk_widget_hide(GTK_WIDGET(file_dialog));
}

void hide_set_days_dialog()
{
	const char *mo = "Mo";
	const char *tu = "Tu";
	const char *we = "We";
	const char *th = "Th";
	const char *fr = "Fr";

	const char *labels[5] = {mo, tu, we, th, fr};
	GtkToggleButton *buttons[5] = {GTK_TOGGLE_BUTTON(monday), GTK_TOGGLE_BUTTON(tuesday), GTK_TOGGLE_BUTTON(wednesdy), GTK_TOGGLE_BUTTON(thursday), GTK_TOGGLE_BUTTON(friday)};
	char out[11];
	out[0] = '\0';

	for(i = 0; i < 5; i++)
	{
		if(days[i] == 1)
		{
			strcat(out, labels[i]);
			gtk_toggle_button_set_active(buttons[i], 0);
		}
		days[i] = 0;
	}
	gtk_tree_selection_get_selected(selector, &model, &iter);
	gtk_list_store_set(store, &iter,
                        COL_DAYS, out,
                                    -1);

    gtk_widget_hide(GTK_WIDGET(set_days_window));
}

void hide_error_dialog()
{
	gtk_widget_hide(GTK_WIDGET(error_dialog));
}

void hide_file_dialog()
{
	gtk_widget_hide(GTK_WIDGET(file_dialog));
}

void generate_sections()
{
	int num_sections = gtk_spin_button_get_value_as_int(spin_button);
	gtk_tree_selection_set_mode(selector, GTK_SELECTION_SINGLE);
	gtk_tree_selection_get_selected(selector, &model, &iter);

	if(selector == 0)
		return;

	char *dept, *num, *days, *bldg, *instr;
	int start, end, sect, room;

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

	int i;
	for(i = 0; i < num_sections; i++)
	{
		gtk_list_store_insert_before(store, &iter, 0);

		gtk_list_store_set(store, &iter,
		COL_DEPT, dept,
		COL_NUMBER, num,
		COL_START, start,
		COL_END, end,
		COL_DAYS, days,
		COL_SECT, i+2,
		COL_BLDG, bldg,
		COL_ROOM, room,
		COL_INSTR, instr,
		-1);

		gtk_tree_model_iter_next(model, &iter);
	}


	gtk_widget_hide(GTK_WIDGET(generate_sections_dialog));

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


//Get user input from the cell and update the rowquite character c
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
			if(!is_int(new_text) && atoi(new_text) < 700 && atoi(new_text) > 2200)
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
		break;
		case COL_ROOM:
		{
			if(is_int(new_text) && atoi(new_text) > -1)
			{
				gint entry = atoi(new_text);
                gtk_list_store_set(store, &iter,
								COL_ROOM, entry,
								-1);
			}
			else
			{
				gtk_message_dialog_format_secondary_markup(error_dialog, "%s", INVALID_ROOM_ERROR);
				gtk_dialog_run(GTK_DIALOG(error_dialog));
			}
		}
		break;
	}

	gtk_tree_path_free(path);

}

//Sort number based columns
GtkTreeIterCompareFunc sort_by_num(GtkTreeViewColumn *acolumn, gpointer data)
{
	GtkTreeSortable *sortable;
	GtkSortType order;
	gint sort_id;

	sortable = GTK_TREE_SORTABLE(model);

	gint col_num = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (acolumn), "column"));
	gtk_tree_sortable_set_sort_column_id(sortable, col_num, GTK_SORT_ASCENDING);

	printf("MADE IT HERE\n");
	return 0;
}

GtkTreeIterCompareFunc sort_by_alpha(GtkTreeViewColumn *acolumn, gpointer data)
{

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

	//Increment the iterator. If this is not done, the program will crash upon certain instructions.
	gtk_tree_model_iter_next(model, &iter);
}
