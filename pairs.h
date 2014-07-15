#pragma once

char *main_menu_labels[] =
{
    "Enter data for class schedules",
    "Enter faculty data",
    "Reports menu",
    "Create new semester",
    "About",
    " ",
    "Exit"
};

char *class_menu_labels[] =
{
    "Add new class",
    "Edit or view an existing course",
    "Delete an existing course",
    "Generate section numbers for the enire schedule",
    "Generate section numbers for one course",
    " ",
    "Back"
};

char * fac_menu_labels[] =
{
    "Add a faculty member record",
    "Edit view or delete an existing record",
    "Recall a deleted record and edit it",
    " ",
    "Back"
};

char * reports_menu_labels[] =
{
    "test",
    "test",
    "test"
};

DMENU class_menu = {"Class Scheduling", class_menu_labels};
DMENU fac_menu = {"Faculty", fac_menu_labels};
DMENU reports_menu = {"Reports", reports_menu_labels};
