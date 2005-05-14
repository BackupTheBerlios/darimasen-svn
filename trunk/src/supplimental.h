/* Darimasen - supplimental.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef SUPPLIMENTAL_H
#define SUPPLIMENTAL_H

/**********************/

#include "main.h"

/**********************/

Glib::ustring int2ustr(int);
Glib::ustring underscoreSafe(Glib::ustring);
Glib::ustring getchoicesdir();
void goruncommand(Glib::ustring, Glib::ustring);
Glib::ustring trim_whitespace(const Glib::ustring&);

/**********************/

#endif //SUPPLIMENTAL_H

