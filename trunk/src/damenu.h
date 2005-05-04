/* Darimasen - darimasen.h - Copyright (C) 2004 - 2005 Sudrien, GPL
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


#ifndef DAMENU_H
#define DAMENU_H


#include <gtkmm.h>
#include "darimasen.h"
#include <iostream>

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
