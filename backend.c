#include <sqlite3.h>

char* open_db(char *name);
char* create_db();
void close_db();

sqlite3 *working_db;

char* open_sqlite(char *name)
{
	int error = sqlite3_open(name, &db)
	if(error)
	{
		return sqlite3_errmsg(db);
	}
	return "";
}

void close_db()
{
	sqlite3_close(db);
}
