#include "sqlite3.h"
#include "constants.h"
#include <stdlib.h>

char *open_db(char *name);
char *create_db();
char *open_db();
void close_db();
void free_back();

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

char sem_name[15];

char *dept_vals[MAX_ROWS];
char *num_vals[MAX_ROWS];
char *day_vals[MAX_ROWS];
char *bldg_vals[MAX_ROWS];
char *instr_vals[MAX_ROWS];
int start_vals[MAX_ROWS];
int end_vals[MAX_ROWS];
int sect_vals[MAX_ROWS];
int room_vals[MAX_ROWS];

int idx;
int rows;

sqlite3 *working_db;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	if(argc == 1)
	{
		strcpy(sem_name, argv[0]);
		return 0;
	}


	int i = 0;
	rows = 0;
	while(i < argc)
	{
		dept_vals[idx] = malloc(sizeof(char*)*sizeof(argv[i]));
		strcpy(dept_vals[idx],argv[i++]);

		num_vals[idx] = malloc(sizeof(char*)*sizeof(argv[i]));
		strcpy(num_vals[idx],argv[i++]);

		start_vals[idx] = atoi(argv[i++]);
		end_vals[idx] = atoi(argv[i++]);
		day_vals[idx] = malloc(sizeof(char*)*sizeof(argv[i]));
		strcpy(day_vals[idx],argv[i++]);

		sect_vals[idx] = atoi(argv[i++]);
		bldg_vals[idx] = malloc(sizeof(char*)*sizeof(argv[i]));
		strcpy(bldg_vals[idx],argv[i++]);

		room_vals[idx] = atoi(argv[i++]);
		instr_vals[idx] = malloc(sizeof(char*)*sizeof(argv[i]));
		strcpy(instr_vals[idx],argv[i++]);

		idx++;
		rows++;

	}
	return 0;
}


//NOT DONE
char *create_db(char *name, char *table)
{
	printf("%s\n",name);

	int error = sqlite3_open(name, &working_db);
	char *message;
	char sql[150];

	if(error != SQLITE_OK)
	{
		return (char *) sqlite3_errmsg(working_db);
	}

	sprintf(sql ,	"CREATE TABLE %s("\
					"Dept varchar(3),"\
					"Num varchar(4),"\
					"Start smallint,"\
					"End smallint,"\
					"Days varchar(10),"\
					"Sect tinyint,"\
					"Bldg char(3),"\
					"Room tinyint,"\
					"Inst varchar(255));",table);

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

	if(error != 0)
		printf("Could not execute \"%s\" error %d: %s\n",statement,error,message);
	return message;
}

char *open_db(char *db_name)
{
	int error = sqlite3_open(db_name, &working_db);

	if(error != SQLITE_OK)
		return (char *) sqlite3_errmsg(working_db);
	else
		return "ok";
}

void close_db()
{
	sqlite3_close(working_db);
}

void free_back()
{
    int i;
    for(i = 0; i < rows; i++)
    {
        free(dept_vals[i]);
        free(num_vals[i]);
        free(day_vals[i]);
        free(bldg_vals[i]);
        free(instr_vals[i]);
    }
}

//Get values
char *get_name(){return sem_name;}

char **get_dept_vals(){return dept_vals;}
char **get_num_vals(){return num_vals;}
char **get_day_vals(){return day_vals;}
char **get_bldg_vals(){return bldg_vals;}
char **get_instr_vals(){return instr_vals;}
int *get_start_vals(){return start_vals;}
int *get_end_vals(){return end_vals;}
int *get_sect_vals(){return sect_vals;}
int *get_room_vals(){return room_vals;}
int get_size(){return idx;}
