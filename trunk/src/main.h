/* Darimasen - main.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef MAIN_H
#define MAIN_H

/**********************/

#include <gtkmm/main.h>
#include <glibmm/ustring.h>
//#include <gtkmm.h>
//#include <glibmm.h>
// Not sure if this is a legit difference, Rich,
// But I'll leave it here until I can test otherwise

#include "darimasen.h"

/**********************/

#ifdef WIN32
const Glib::ustring slash = "\\";
#else
const Glib::ustring slash = "/";
#endif

const unsigned short AvailableModes = 2;

/**********************/

#endif //MAIN_H
