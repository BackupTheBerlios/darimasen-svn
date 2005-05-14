/* Darimasen - main.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef MAIN_H
#define MAIN_H

/**********************/

#include <vector>
#include <iostream>
#include <libgnomevfsmm.h>
#include <gtkmm.h>

#include "supplimental.h"

/**********************

#include "darimasen.h"
#include "optionsManager.h"
#include "supplimental.h"
#include "iconmodes.h"
#include "damenu.h"
/**********************/

#ifdef WIN32
const Glib::ustring slash = "\\";
#else
const Glib::ustring slash = "/";
#endif

const unsigned short AvailableModes = 2;

/**********************/

#endif //MAIN_H
