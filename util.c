#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_int(const gchar *str);
int is_valid_course_num(const gchar *str);
int add_time(int x, int y);
char *file_extension_correct(char *in);

const char *COURSE_NUMBER_ERROR = "The entry was invalid. A course number can be two or three didgets with an optional letter at the end.";
const char *INVALID_TIME_ERROR = "The entry was invalid. A time must be an integer between 700 and 2200.";
const char *INVALID_ROOM_ERROR = "The entry was invalid. A room number must be an integer greater than -1.";

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

//Adds two integers together as if they were times.
int add_time(int x, int y)
{
	int hourx =  x/100;
	int minutex = x - hourx * 100;
	int houry = y/100;
	int minutey = y - houry * 100;

	int minute_sum = minutex + minutey;
	int hour_sum = hourx + houry;

	if(minute_sum >= 60)
	{
		minute_sum -= 60;
		hour_sum++;
	}

	return hour_sum*100 + minute_sum;
}

char *file_extension_correct(char *in)
{
	int size = strlen(in);
	char *name = malloc(255);

	if(in[size - 3] != '.' || in[size - 2] != 'd' || in[size - 1] != 'b')
	{
		sprintf(name, "%s.db", in);
		return name;
	}
	return in;

}
