#include <sqlite3.h>

char *open_db(char *name);
char *create_db();
void close_db();

sqlite3 *working_db;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for(i = 0; i < argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}


//NOT DONE
char *create_db(char *name)
{

	int error = sqlite3_open(name, &working_db);
	char *message;
	char *sql;

	if(error)
	{
		return sqlite3_errmsg(working_db);
	}
	return "";

	working_db =	"CREATE TABLE CoB_Sched("\
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

void execute_sql(char *statement)
{
	char *message;
	int error = sqlite3_exec(working_db, statement, callback, 0, &message);

	if(error != SQLITE_OK)
	{
		printf("* * * * * SQL Error: %s\n",&message);
		sqlite3_free(message);
	}
}

void close_db()
{
	sqlite3_close(working_db);
}
