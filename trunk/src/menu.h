/* Darimasen - menu.h - Copyright (C) 2005 Sudrien, GPL */

#ifndef MENU_H
#define MENU_H

#include <glibmm.h>
#include <gtkmm.h>
#include <libgnomevfsmm.h>
#include <iostream>
#include "darimasen.h"

class darimasen_menu : public Gtk::Toolbar{
  public:
    darimasen_menu(class darimasen_window&);

  private:
    class darimasen_window * parent;
};
#endif
