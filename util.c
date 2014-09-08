#include <stdlib.h>

int is_int(const gchar *str);

int is_int(const gchar *str)
{
    char *err;
    int out = strtol(str, &err, 10);

    if(! *err)
        return 0;

    return 1;
}
