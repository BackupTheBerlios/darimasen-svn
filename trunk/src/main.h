/* Darimasen - main.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef MAIN_H
#define MAIN_H

/**********************/

#include <gtkmm/main.h>
#include <glibmm/ustring.h>
#include <libgnomevfsmm.h>
#include <vector>
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
