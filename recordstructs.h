#pragma once

#define FACULTY fac_st;
#define COURSE course_stl;

struct fac_st
{
    char *firstname;
    char *middleitl;
    char *lastname;
}

struct course_stl
{
    char *dept;
    char number[3];
    char building[3];

    short room;
    short section

    FACULTY *instructor;
}

struct section
{

}
