/* Darimasen - icon.cpp - Copyright (C) 2005 Sudrien, GPL */

#include "icon.h"


/* ~_~; 

icon::icon(
    const class directory& parent,
    const Glib::ustring& file){

  Glib::RefPtr<const Gnome::Vfs::FileInfo> result;
  icon(parent, result);
  }

/* ~_~; */

icon::icon(
    const class directory& dir,
    Glib::RefPtr<const Gnome::Vfs::FileInfo> file){

  parent = &dir;
  file_info = file;
  }

/* ~_~; */

icon::~icon(){}

/* ~_~; */
