/* Darimasen - darimasen.h - Copyright (C) 2005 Sudrien, GPL */


#ifndef DAWINDOW_H
#define DAWINDOW_H

#include <gtkmm.h>
#include <vector>
#include "tabs.h"
#include "menu.h"

class darimasen_window : public Gtk::Window {
  public:
    darimasen_window(std::vector<Glib::ustring> );
    ~darimasen_window();
  private:
  public:
    class tabs * tabber;
    class darimasen_menu * da_menu;
  };

#endif
