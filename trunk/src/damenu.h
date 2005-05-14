/* Darimasen - damenu.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef DAMENU_H
#define DAMENU_H

/**********************/

#include "main.h"
#include "darimasen.h"

/**********************/

  class DarimasenMenu : public Gtk::MenuBar {

    class Darimasen * parent;

    int depth;

    Gtk::Menu ** MenuArray; 
    Gtk::MenuItem ** MenuItemArray;
    Glib::ustring * menulevel;

    Gtk::Menu prompt;

    void copy(Glib::ustring);
    void move(Glib::ustring);
    void link(Glib::ustring);
    void unlinkify(Glib::ustring);

    void bookmark(Glib::ustring);

    void MenuForPath(int, Glib::ustring , Glib::ustring); //extension
    bool SpecialMenuForPath(GdkEventButton* , int, Glib::ustring  , Glib::ustring); //extension

    Glib::ustring CountSubdir(const Glib::ustring&);
    bool DaMenuSelect(GdkEventButton*, const Glib::ustring,guint,bool);
    void selection_reset(guint, Glib::ustring);
 
    bool * needsRebuild;
  public:

    void signal_deactivate ();
    DarimasenMenu(const Glib::ustring&, class Darimasen&, guint);
    ~DarimasenMenu();

    };

/**********************/

#endif //DAMENU_H
