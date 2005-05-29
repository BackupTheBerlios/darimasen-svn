/* Darimasen - icon.cpp - Copyright (C) 2005 Sudrien, GPL */

#include "icon.h"


/* ~_~; */

icon::icon(){
    add(icon_pixmap);
    add(file_name);
    add(file_mime);
    add(file_size);
    add(file_permissions);
    add(file_owner);
    add(file_group);
    add(file_hidden);
  }

/* ~_~; */

icon::~icon(){}

/* ~_~; */
