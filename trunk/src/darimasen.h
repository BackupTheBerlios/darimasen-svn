/* Darimasen - darimasen.h - Copyright (C) 2005 Sudrien, GPL */


#ifndef DAWINDOW_H
#define DAWINDOW_H

#include <gtkmm.h>
#include <vector>
#include "tabs.h"

class darimasen_window : public Gtk::Window {
  public:
    darimasen_window(std::vector<Glib::ustring> );
    ~darimasen_window();


  };

#endif
