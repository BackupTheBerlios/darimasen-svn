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


#ifndef DARIMASEN_H
#define DARIMASEN_H

/**********************/

#include <gtkmm/window.h>
#include <stack>
#include <gtkmm/box.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/separatortoolitem.h>
#include <gtkmm/toggletoolbutton.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/menubar.h>
#include <gtkmm/paned.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/menu.h>
#include <gtkmm/stock.h>


#include "main.h"
#include <sys/types.h>
#include <gtkmm/eventbox.h>

#include <gtkmm/menuitem.h>


#include "supplimental.h"
#include <iostream>
#include <gtkmm/scrollbar.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/notebook.h>

#include <gdkmm/pixbufloader.h>

#include <gtkmm/layout.h>

#include "iconmodes.h"
/**********************/

class Darimasen : public Gtk::Window {
  class DarimasenMenu : public Gtk::MenuBar {

    Darimasen * parent;

    int depth;

    Gtk::Menu ** MenuArray; 
    Gtk::MenuItem ** MenuItemArray;
    Glib::ustring * menulevel;

    void MenuForPath(int, Glib::ustring, Glib::ustring);
    void SpecialMenuForPath(int, Glib::ustring, Glib::ustring);

    void offClick(int, Glib::ustring);

    Glib::ustring CountSubdir(const Glib::ustring&);
    bool DaMenuSelect(GdkEventButton*, const Glib::ustring);
 
//sigc::slot<void, int&, int&, bool&, sigc::nil, sigc::nil, sigc::nil, sigc::nil>

  //  Glib::SignalProxy3<void, int&, int&, bool&>signal_popup_menu_position();


//sigc::slot<void, int&, int&, bool&, sigc::nil, sigc::nil, sigc::nil, sigc::nil> (void on_popup_menu_position(int&, int&, bool&));
  //  void on_popup_menu_position(int&, int&, bool&);
    bool usingSpecial;
//void on_popup_menu_position(int& x, int& y, bool& push_in);
//typedef on_popup_menu_position Gtk::Menu::SlotPositionCalc;

  public:
    DarimasenMenu(const Glib::ustring, Darimasen&);
    ~DarimasenMenu();
    };



  Gtk::VBox VerticalOrganizer;
  Gtk::Toolbar TopBar;
  Gtk::ToolItem * DarimasenMenuContainer;
  DarimasenMenu * DaMenu;
  Gtk::HPaned HideTreePane;
  Gtk::ScrolledWindow TreeScroller;
  Gtk::Notebook * Tabber;
  Gtk::Statusbar Info;


  std::vector< std::stack<Glib::ustring> > history;
  Gtk::ToolButton * BackButton;

  void fNewTab();
  void newTab(Glib::ustring);
  void tabberSwitched(GtkNotebookPage*, guint);
  void addTab(guint);
  void ChangeCurrentPath(Glib::ustring path);
  void removeTab(guint);
  void fBack();
  void fQuit();
  void fAbout();
  void fShowHidden();
  void fPrintHist();


  std::vector< class DaIconModes* > IconModeList;


public:
  Darimasen(std::vector<Glib::ustring>);
  ~Darimasen();
  Gtk::CheckMenuItem * optShowHidden;
  void set_message(Glib::ustring);
  };

/**********************/

#endif //DARIMASEN_H




