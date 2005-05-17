/* Darimasen - supplimental.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef SUPPLIMENTAL_H
#define SUPPLIMENTAL_H

/**********************/

#include "main.h"

/**********************/

Glib::ustring int2ustr(int);

Glib::ustring get_choices_dir();

void exec_command(Glib::ustring, Glib::ustring);

Glib::ustring trim_whitespace(const Glib::ustring&);

Glib::ustring underscore_safe(Glib::ustring);

/**********************/

#endif //SUPPLIMENTAL_H

