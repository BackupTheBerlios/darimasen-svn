/* Darimasen - iconmodes.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef ICONMODES_H
#define ICONMODES_H

/**********************/

#include <gtkmm/table.h>
#include <dirent.h>
#include <sys/types.h>
#include "sidecon.h"
#include "main.h"

/**********************/

class DaIconModes : public Gtk::Table {

  Glib::ustring fullPath;
  int filesAtPath;
  int heightAvailable;
  unsigned short iconmode;
  bool showHidden;
  
  void DaSideconLister();
  void DaDetailLister();

public:

  DaIconModes(Glib::ustring, int, unsigned short, int, bool); //path, filesAtPath, mode 
   
  };

/**********************/

#endif //ICONMODES_H
