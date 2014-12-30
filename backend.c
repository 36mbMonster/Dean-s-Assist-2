#include "sqlite3.h"

char *open_db(char *name);
char *create_db();
char *open_db();
void close_db();


char **get_dept_vals();
char **get_num_vals();
char **get_day_vals();
char **get_bldg_vals();
char **get_instr_vals();
int *get_start_vals();
int *get_end_vals();
int *get_sect_vals();
int *get_room_vals();

int get_size();

char *dept_vals[255];
char *num_vals[255];
char *day_vals[255];
char *bldg_vals[255];
char *instr_vals[255];
int start_vals[255];
int end_vals[255];
int sect_vals[255];
int room_vals[255];

int idx;

sqlite3 *working_db;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i = 0;
	while(i < argc)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		dept_vals[idx] = argv[i++];
		num_vals[idx] = argv[i++];
		start_vals[idx] = atoi(argv[i++]);
		end_vals[idx] = atoi(argv[i++]);
		day_vals[idx] = argv[i++];
		sect_vals[idx] = atoi(argv[i++]);
		bldg_vals[idx] = argv[i++];
		room_vals[idx] = atoi(argv[i++]);
		instr_vals[idx] = argv[i++];
		printf("*****: %s %s %d %d %s %d %s %d %s\n",dept_vals[idx], num_vals[idx], start_vals[idx], end_vals[idx], day_vals[idx], sect_vals[idx], bldg_vals[idx], room_vals[idx], instr_vals[idx]);
		//i=0;
		idx++;

	}
	printf("\nargc was %d\n",argc);
	return 0;
}


//NOT DONE
char *create_db(char *name)
{
	printf("%s\n",name);

	int error = sqlite3_open(name, &working_db);
	char *message;
	char *sql;

	if(error != SQLITE_OK)
	{
		return sqlite3_errmsg(working_db);
	}

	sql =	"CREATE TABLE CoB_Sched("\
					"Dept varchar(3),"\
					"Num varchar(4),"\
					"Start smallint,"\
					"End smallint,"\
					"Days varchar(10),"\
					"Sect tinyint,"\
					"Bldg char(3),"\
					"Room tinyint,"\
					"Inst varchar(255));";

	error = sqlite3_exec(working_db, sql, callback, 0, &message);

	if(error != SQLITE_OK)
		return message;
	else
		return "ok";

}

char *execute_sql(char *statement)
{
	char *message;
	idx = 0;
	int error = sqlite3_exec(working_db, statement, callback, 0, &message);

	printf("error: %d\n",error);

	if(error != SQLITE_OK)
		return message;
}

char *open_db(char *db_name)
{
	int error = sqlite3_open(db_name, &working_db);

	if(error != SQLITE_OK)
		return sqlite3_errmsg(working_db);
	else
		return "ok";
}

void close_db()
{
	sqlite3_close(working_db);
}

//Get values
char **get_dept_vals(){return dept_vals;}
char **get_num_vals(){return num_vals;}
char **get_day_vals(){printf("----------->%d\n",sect_vals[1]);return day_vals;}
char **get_bldg_vals(){return bldg_vals;}
char **get_instr_vals(){return instr_vals;}
int *get_start_vals(){return start_vals;}
int *get_end_vals(){return end_vals;}
int *get_sect_vals(){return sect_vals;}
int *get_room_vals(){return room_vals;}
int get_size(){return idx;}
