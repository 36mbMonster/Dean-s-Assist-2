#include <stdlib.h>

int is_int(const gchar *str);

const char *COURSE_NUMBER_ERROR = "The entry was invalid. A course number can be two or three didgets with an optional letter at the end.";

int is_int(const gchar *str)
{
    char *err;
    int out = strtol(str, &err, 10);

    if(! *err)
        return 0;

    return 1;
}
