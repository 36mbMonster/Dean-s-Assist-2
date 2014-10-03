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

char *open_sqlite(char *name)
{
	int error = sqlite3_open(name, &working_db)
	if(error)
	{
		return sqlite3_errmsg(working_db);
	}
	return "";
}

//NOT DONE
char *create_db()
{
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
	int error;
	char *message;
	char *sql;

	error = sqlite3_exec(working_db, sql, callback, 0, &message);

	if(error != SQLITE_OK)
		return message;
	else
		return "ok";

}

void close_db()
{
	sqlite3_close(db);
}
