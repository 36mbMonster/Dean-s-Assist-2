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
	about_dialog = GTK_ABOUT_DIALOG(gtk_builder_get_object(builder,"aboutdialog"));
	error_dialog = GTK_MESSAGE_DIALOG(gtk_builder_get_object(builder,"error_dialog"));
	generate_sections_dialog = GTK_MESSAGE_DIALOG(gtk_builder_get_object(builder, "gensect_dialog"));
	semester_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "semester_dialog"));

	//Load menu items
	about_item = gtk_builder_get_object(builder, "about");
	quit_item = gtk_builder_get_object(builder, "quit");
	show_hide_columns_item = gtk_builder_get_object(builder, "show_hide_columns");
	new_course_item = gtk_builder_get_object(builder, "new_course");
	delete_row_item = gtk_builder_get_object(builder, "delete");
	save_semester_item = gtk_builder_get_object(builder,"save_semester");
	save_semester_as_item = gtk_builder_get_object(builder, "save_semester_as");
	load_semester_item = gtk_builder_get_object(builder, "load_semester");
	generate_sections_item = gtk_builder_get_object(builder, "generate_sections");
	print_item = gtk_builder_get_object(builder, "print");
	new_semester_item = gtk_builder_get_object(builder, "new_semester");
	set_semester_item = gtk_builder_get_object(builder, "meta");

    //Load check boxes
    monday = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "mo"));
    tuesday = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "tu"));
    wednesdy = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "we"));
    thursday = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "th"));
    friday = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "fr"));

	time_offset = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "time_offset_check"));

	//Load buttons
	error_okay_button = GTK_BUTTON(gtk_builder_get_object(builder,"error_dialog_okay"));
	days_okay_button = GTK_BUTTON(gtk_builder_get_object(builder,"set_days_okay"));
	sect_gen_okay_button = GTK_BUTTON(gtk_builder_get_object(builder, "sect_gen_okay_button"));
	spin_button = GTK_SPIN_BUTTON(gtk_builder_get_object(builder,"spinbutton"));
	spin_button1 = GTK_SPIN_BUTTON(gtk_builder_get_object(builder,"spinbutton1"));

	//Load tree and list related structures.
	treeview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeview"));
	selector = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "treeview-selection"));
	store = GTK_LIST_STORE(gtk_builder_get_object(builder,"liststore"));

	//Set the tree model
	model = gtk_tree_view_get_model(treeview);

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
		gtk_tree_view_column_set_sort_column_id(columns[i-1], i-1);
	}

	//treeview = GTK_TREE_VIEW(gtk_tree_view_new_with_model(model));

	//Put selector in single select mode
	selector = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	gtk_tree_selection_set_mode(selector, GTK_SELECTION_SINGLE);

	//Make the font bigger
	//If you don't use a generated font description, the text won't render.
	PangoFontDescription *font;
	font = pango_font_description_from_string("Arial 14");
	gtk_widget_override_font (GTK_WIDGET(window), font);

	spin_adjust = gtk_adjustment_new(0, 0, MAX_GEN_SECTIONS, 1, 2, 0);
	gtk_spin_button_set_adjustment(spin_button, spin_adjust);

	/*spin_adjust1 = gtk_adjustment_new(get_current_year(), 2000, 3000, 1, 5, 0);
	gtk_spin_button_set_adjustment(spin_button1, spin_adjust1);*/

	has_saved = 0;
	unsaved_changes = 0;

/**
***************************************************************************
*							  Connect Signals						  	  *
***************************************************************************
*/

	//Connect menu signals
	g_signal_connect(window, "delete_event", G_CALLBACK(quit_deans2), NULL);
	g_signal_connect(about_item, "activate", G_CALLBACK(show_about_dialog), NULL);
	g_signal_connect(quit_item, "activate", G_CALLBACK(quit_deans2), NULL);
	g_signal_connect(new_course_item, "activate", G_CALLBACK(new_row), NULL);
	g_signal_connect(delete_row_item, "activate", G_CALLBACK(delete_row), NULL);
	g_signal_connect(save_semester_item, "activate", G_CALLBACK(save), NULL);
	g_signal_connect(save_semester_as_item, "activate", G_CALLBACK(save_as), NULL);
	g_signal_connect(load_semester_item, "activate", G_CALLBACK(load_file), NULL);
	g_signal_connect(show_hide_columns_window, "hide", G_CALLBACK(hide_adjust_columns_dialog), NULL);
	g_signal_connect(generate_sections_item, "activate", G_CALLBACK(show_generate_sections), NULL);
	g_signal_connect(print_item, "activate", G_CALLBACK(prep_printer), NULL);
	g_signal_connect(new_semester_item, "activate", G_CALLBACK(new_semester),NULL);
	g_signal_connect(set_semester_item, "activate", G_CALLBACK(set_semester), NULL);

	//Connect button signals
	g_signal_connect(error_okay_button, "clicked", G_CALLBACK(hide_error_dialog), NULL);
	g_signal_connect(days_okay_button, "clicked", G_CALLBACK(hide_set_days_dialog), NULL);
	g_signal_connect(sect_gen_okay_button, "clicked", G_CALLBACK(generate_sections), NULL);

    //Connect check box signals
    g_signal_connect(monday, "clicked", G_CALLBACK(check_clicked), NULL);
    g_signal_connect(tuesday, "clicked", G_CALLBACK(check_clicked), NULL);
    g_signal_connect(wednesdy, "clicked", G_CALLBACK(check_clicked), NULL);
    g_signal_connect(thursday, "clicked", G_CALLBACK(check_clicked), NULL);
    g_signal_connect(friday, "clicked", G_CALLBACK(check_clicked), NULL);

    g_signal_connect(time_offset, "clicked", G_CALLBACK(time_gen), NULL);

	//Connect cell renderer signals.
	g_signal_connect(dept_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(number_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(start_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(end_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(days_text, "editing-started", G_CALLBACK(edit_days), NULL);
	g_signal_connect(sect_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(bldg_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(room_text, "edited", G_CALLBACK(cell_edited), NULL);
	g_signal_connect(instructor_text, "edited", G_CALLBACK(cell_edited), NULL);

	//Connect keyboard event signal.
	g_signal_connect(window, "key_release_event", G_CALLBACK(key_handler), NULL);

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
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600); //fix magic number
    gtk_widget_show(window);
    gtk_main();

    return 0;
}
/**
***************************************************************************
*							Signal Functions						  	  *
***************************************************************************
*/

//Hot key event handling
void key_handler(GtkWidget *win, GdkEventKey *event, gpointer data)
{
	//printf("%s\n",gdk_keyval_name(event->keyval));
	switch(event -> keyval)
	{
		case GDK_KEY_g:
			if(event->state & GDK_CONTROL_MASK)
			{
				show_generate_sections();
				break;
			}
		case GDK_KEY_o:
			if(event->state & GDK_CONTROL_MASK)
			{
				load_file();
				break;
			}
		case GDK_KEY_p:
			if(event->state & GDK_CONTROL_MASK)
			{
				prep_printer();
				break;
			}
		case GDK_KEY_s:
		case GDK_KEY_S:
			if(event->state & GDK_CONTROL_MASK)
			{
				save();
				break;
			}
			else if(event->state & GDK_CONTROL_MASK & GDK_SHIFT_MASK)
			{
				save_as();
				break;
			}
		case GDK_KEY_F4:
			if(event->state & GDK_MOD1_MASK)
			{
				quit_deans2();
				break;
			}
		case GDK_KEY_plus:
		case GDK_KEY_KP_Add:
			if(event->state & GDK_CONTROL_MASK)
			{
				new_row();
				break;
			}
		default:
			return;
	}
}

void quit_deans2()
{
	printf("%d\n",unsaved_changes);
	if(unsaved_changes)
	{
		GtkWidget *question_dialog;
		question_dialog = gtk_message_dialog_new(GTK_WINDOW (window), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
												 "Quitting deans2 will result in the loss of any unsaved work in the current semester.\nAre you sure you want to quit?");

		int res = gtk_dialog_run(GTK_DIALOG(question_dialog));

		if(res == GTK_RESPONSE_YES)
		{
			free_back();
			gtk_main_quit();
			return;
		}
		else
		{
			gtk_widget_destroy(question_dialog);
			return;
		}

		gtk_widget_destroy(question_dialog);
	}
	free_back();

	gtk_main_quit();

}

void show_about_dialog()
{
	//gtk_widget_show_all( about_dialog );
	gtk_dialog_run(GTK_DIALOG(about_dialog));
	gtk_widget_hide(GTK_WIDGET(about_dialog));
}

void set_semester()
{
	gtk_dialog_run(semester_dialog);

}

void cut()
{
    //copy();
    //delete here
}

//Copy one row.
void copy()
{
    //Copy row data into temporary variables.

	GtkTreeModel *model2;
    GtkTreeIter iter2;

    model2 = gtk_tree_view_get_model(treeview);
    gtk_tree_selection_get_selected(selector, &model2, &iter2);

    char *dept, *num, *days, *bldg, *instr;
	int start, end, sect, room;

	gtk_tree_model_get (model2, &iter2, COL_DEPT, &dept, -1);
	gtk_tree_model_get (model2, &iter2, COL_NUMBER, &num, -1);
	gtk_tree_model_get (model2, &iter2, COL_DAYS, &days, -1);
	gtk_tree_model_get (model2, &iter2, COL_BLDG, &bldg, -1);
	gtk_tree_model_get (model2, &iter2, COL_INSTR, &instr, -1);
	gtk_tree_model_get (model2, &iter2, COL_START, &start, -1);
	gtk_tree_model_get (model2, &iter2, COL_END, &end, -1);
	gtk_tree_model_get (model2, &iter2, COL_SECT, &sect, -1);
	gtk_tree_model_get (model2, &iter2, COL_ROOM, &room, -1);
}

void paste()
{
    //insert row
    //clear 'clipboard'
}

void new_semester()
{
    //Check whether or not the model is empty
    if(gtk_tree_model_get_iter_first(model, &iter))
    {
        GtkWidget *question_dialog;
        question_dialog = gtk_message_dialog_new(GTK_WINDOW (window), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                                 "Creating a new semester will result in the loss of any unsaved work in the current semester.\nAre you sure you want to continue?");

        int res = gtk_dialog_run(GTK_DIALOG(question_dialog));

        if(res == GTK_RESPONSE_YES)
        {
            gtk_list_store_clear(store);
            treeview = GTK_TREE_VIEW(gtk_tree_view_new());
            model = gtk_tree_view_get_model(treeview);
        }

        gtk_widget_destroy(question_dialog);
    }
}

void save()
{
    if(has_saved)
    {
    	open_db(filename);
    	char *statement = "delete from CoB_Sched;";
    	execute_sql(statement);
    	close_db();
    	write_to_db(filename);
    	unsaved_changes = 0;
    }
    else
        save_as();
}

void save_as()
{
	file_dialog = GTK_FILE_CHOOSER_DIALOG(gtk_file_chooser_dialog_new (
									  "Save File",
                                      GTK_WINDOW(window),
                                      GTK_FILE_CHOOSER_ACTION_SAVE,
                                      ("_Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("_Save"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL));

	int response = gtk_dialog_run(GTK_DIALOG(file_dialog));

	if(response == GTK_RESPONSE_ACCEPT)
	{

		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_dialog));
		write_to_db(filename);
		gtk_window_set_title(GTK_WINDOW(window), filename);

		//set flags
		has_saved = 1;
		unsaved_changes = 0;
	}

	gtk_widget_destroy(GTK_WIDGET(file_dialog));
}

void load_file()
{

	GtkTreeModel *model2;
    GtkTreeIter iter2;

    model2 = gtk_tree_view_get_model(treeview);

	file_dialog = GTK_FILE_CHOOSER_DIALOG(gtk_file_chooser_dialog_new (
								  "Load File",
								  GTK_WINDOW(window),
								  GTK_FILE_CHOOSER_ACTION_OPEN,
								  ("_Cancel"),
								  GTK_RESPONSE_CANCEL,
								  ("_Open"),
								  GTK_RESPONSE_ACCEPT,
								  NULL));

	int response = gtk_dialog_run(GTK_DIALOG(file_dialog));

	if(response == GTK_RESPONSE_ACCEPT)
	{
		//Check whether or not the model is empty
		if(gtk_tree_model_get_iter_first(model2, &iter2))
		{
			GtkWidget *question_dialog;
			question_dialog = gtk_message_dialog_new(GTK_WINDOW (window), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
													 "Loading a semester will result in the loss of any unsaved work in the current semester.\nAre you sure you want to continue?");

			int res = gtk_dialog_run(GTK_DIALOG(question_dialog));

			if(res == GTK_RESPONSE_YES)
			{
				gtk_list_store_clear(store);
			}
			else
			{
				gtk_widget_destroy(question_dialog);
				gtk_widget_destroy(GTK_WIDGET(file_dialog));
				return;
			}

			gtk_widget_destroy(question_dialog);
		}
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_dialog));
		read_from_db(filename);
		gtk_window_set_title(GTK_WINDOW(window), filename);
		has_saved = 1;
	}

	gtk_widget_destroy(GTK_WIDGET(file_dialog));
}

void show_generate_sections()
{
	GtkTreeModel *model2;
    GtkTreeIter iter2;

    model2 = gtk_tree_view_get_model(treeview);
	selector = gtk_tree_view_get_selection(treeview);

	if(gtk_tree_model_get_iter_first(model2, &iter2) && gtk_tree_selection_get_selected(selector, &model2, &iter2))
		gtk_widget_show_all(GTK_WIDGET(generate_sections_dialog));
	else
	{
		GtkWidget *dialog;
		dialog = gtk_message_dialog_new(GTK_WINDOW (window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
			"There is no class section selected to use for generation.");

		int res = gtk_dialog_run(GTK_DIALOG(dialog));

		if(res == GTK_RESPONSE_OK)
		{
			gtk_widget_destroy(dialog);
		}
	}
}

void show_adjust_columns_dialog()
{
	gtk_widget_show_all(show_hide_columns_window);
}

void hide_adjust_columns_dialog()
{
	gtk_widget_hide(GTK_WIDGET(show_hide_columns_window));
}

void prep_printer()
{
	start_printer(window,model);
}

void write_to_db(char *db_name)
{
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

void read_from_db(char *filename)
{
	char **dept, **num, **days, **bldg, **instr;
	int *start, *end, *sect, *room;
	int i, size;

	printf("%s\n",open_db(filename));
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

	gtk_tree_model_get_iter_first(model, &iter);
	size = get_size();

    for(i = 0; i < size; i++)
    {
       gtk_list_store_insert_with_values(store, &iter, 0,
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
    unsaved_changes = 0;
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

	//Time generation related features
	int diff = 0;
	int gap = 0; //The number of minutes between each generated section.

	if(gen_times)
	{
		diff = end - start;
		gap = 15;
		int p_end = end;

		int i;
		for(i = 0; i < num_sections; i++)
		{
			gtk_list_store_insert(store, &iter, 0);

			gtk_list_store_set(store, &iter,
			COL_DEPT, dept,
			COL_NUMBER, num,
			COL_START, add_time(p_end,gap),
			COL_END, add_time(p_end,add_time(gap,diff)),
			COL_DAYS, days,
			COL_SECT, i+2,
			COL_BLDG, bldg,
			COL_ROOM, room,
			COL_INSTR, instr,
			-1);

			gtk_tree_model_iter_next(model, &iter);

			p_end = add_time(p_end,add_time(gap,diff));
		}
	}

	//If the user doesn't want to generate times
	else
	{
		int i;
		for(i = 0; i < num_sections; i++)
		{
			gtk_list_store_insert(store, &iter, 0);

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
	}


	gtk_widget_hide(GTK_WIDGET(generate_sections_dialog));

}

void check_clicked(GtkCheckButton *check,
                    const gchar         *path_string,
                    gpointer             data)
{

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

void time_gen()
{
	gen_times = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(time_offset));
}

//Call this instead of cell_edited when the days column is clicked.
//I needed a separate function because that column gives off an
//"editing started" signal rather than an "edited" signal like the others.
void
edit_days (GtkCellRenderer *cell,
                      GtkCellEditable *editable,
                      const gchar     *path_string,
                      gpointer         data)

{
	path = gtk_tree_path_new_from_string (path_string);

	gtk_tree_model_get_iter (model, &iter, path);

	gtk_widget_show_all(set_days_window);

}

//Get user input from the cell and update the rowquite character c
//WORKS!!
void cell_edited(GtkCellRendererText *renderer,
				const gchar         *path_string,
             	const gchar         *new_text,
             	gpointer             data)
{
	unsaved_changes = 1;

	gint column = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (renderer), "column"));
	path = gtk_tree_path_new_from_string (path_string);

	gtk_tree_model_get_iter (model, &iter, path);

	switch(column)
	{
		case COL_DEPT:
		{
			gchar *old_entry;

			gtk_tree_model_get(model, &iter, column, &old_entry, -1);
			g_free(old_entry);

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
			if(!is_int(new_text) && atoi(new_text) < EARLIEST_CLASS && atoi(new_text) > LATEST_CLASS)
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
			if(!is_int(new_text) && atoi(new_text) < EARLIEST_CLASS && atoi(new_text) > LATEST_CLASS)
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
		case COL_SECT:
        {
                gint entry = atoi(new_text);
                gtk_list_store_set(store, &iter,
								COL_SECT, entry,
								-1);
        }
        break;
		case COL_BLDG:
        {
                gtk_list_store_set(store, &iter,
								COL_BLDG, new_text,
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
        case COL_INSTR:
		{
                gtk_list_store_set(store, &iter,
								COL_INSTR, new_text,
								-1);
		}
		break;
	}


}

//Deletes course section from the semester.
//WORKS!!
void delete_row()
{

	GtkTreeView *treeview2;
    GtkTreeModel *model2;
    GtkTreeIter iter2;

    treeview2 = GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeview"));
    selector = gtk_tree_view_get_selection(treeview2);
    gtk_tree_selection_get_selected(selector, &model2, &iter2);
    gtk_list_store_remove(GTK_LIST_STORE(store), &iter2);
}

//Adds a new course or section to the list.
//WORKS!!
void new_row()
{

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
