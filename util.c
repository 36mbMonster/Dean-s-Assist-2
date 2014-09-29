#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_int(const gchar *str);
int is_valid_course_num(const gchar *str);

const char *COURSE_NUMBER_ERROR = "The entry was invalid. A course number can be two or three didgets with an optional letter at the end.";
const char *INVALID_TIME_ERROR = "The entry was invalid. A time must be a four digit long integer.";

int is_int(const gchar *str)
{
    char *err;
    strtol(str, &err, 10);

    if(! *err)
        return 1;

    return 0;
}


//This function checks whether or not the course number is valid.
//A valid course number is between 1 & 4 digits.
//The last digit can be a number or a letter, unless the string is 4 digits.
//If the string is 4 digits, the last one MUST be a letter.
int is_valid_course_num(const char *str)
{
    int length = strlen(str);

    //If cell is empty, return true
    if( (length == 0 || length == 1) && (str[0] == ' ' || str[0] == '\0') )
        return 1;

    //If string is between 1 and 4 characters, it may be valid.
    if( length < 5 && length > 0)
    {
        int i;
        for(i = 0; i < length-1; i++)
        {
            if(!isdigit(str[i]))
                return 0;
        }
        //If the last character in the string is not a letter or a number, the string is invalid.
        if(!isalpha(str[length-1]) && !isdigit(str[length-1]))
            return 0;
        //If the first character in the string is a letter, the string is invalid.
        if(isalpha(str[0]))
            return 0;
        //If the length is four, the final character MUST be a letter.
        if(length == 4 && !isalpha(str[length-1]))
            return 0;

        return 1;
    }
    return 0;
}
