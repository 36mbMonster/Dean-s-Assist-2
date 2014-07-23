#pragma once

#define FACULTY struct fac_st;
#define COURSE struct course_stl;
#define SECTION struct section_stl;

struct fac_st
{
    char *firstname;
    char *middleitl;
    char *lastname;

    char* title;
    char* dept;
    int fulltime;

    char* office_bldg;
    short room;
}

struct section_stl
{
    char number[3];
    char building[3];

    short room;

    FACULTY *instructor;

    short stime;
    short etime;
}

struct course_stl
{
    char *dept;
    SECTION *sections;

}
