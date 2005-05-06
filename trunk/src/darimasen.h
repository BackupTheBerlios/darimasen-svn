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
#include "damenu.h"
#include <gtkmm/menutoolbutton.h>


/**********************/

class Darimasen : public Gtk::Window {

  //class DarimasenMenu;

  class aboutDa : public Gtk::Window {
    Gtk::Image * image1;
    Gtk::EventBox * closer;
    bool ch0wned(GdkEventButton*);
  public:
    aboutDa();
    ~aboutDa();


    };
  
  Gtk::VBox VerticalOrganizer;
  Gtk::Toolbar TopBar;
  Gtk::ToolItem * DarimasenMenuContainer;
  class DarimasenMenu * DaMenu;
  Gtk::HPaned HideTreePane;
  Gtk::ScrolledWindow TreeScroller;

  Gtk::Statusbar Info;

  Gtk::ToolItem * CompactMenuContainer;
  Gtk::MenuBar * CompactMenu;
  Gtk::Menu * m_Menu_File;

  Gtk::SeparatorToolItem * sep1;
  Gtk::SeparatorToolItem * sep2;

Gtk::MenuToolButton * BackButton;
Gtk::ToolButton * ChangeIconMode;
 Gtk::ToggleToolButton * ViewTree;
Gtk::ToolButton * NewTab;

  Gtk::Menu * history_menu;

  void fNewTab();

  void tabberSwitched(GtkNotebookPage*, guint);
  void addTab(guint);

  void removeTab(guint);
  void fBack();
  void fQuit();
  void fAbout();
  void fShowHidden();
  void fPrintHist();
  void fChangeIconMode();

Glib::RefPtr<Gdk::Pixbuf> windowIcon;

  std::vector< class DaIconModes* > IconModeList;
  std::vector< class Gtk::EventBox* > EventBoxList;
  short mode;

  std::vector< std::vector<Glib::ustring> > history;




public:

  std::vector <Glib::ustring> mimeList;
  std::vector < Glib::RefPtr<Gdk::Pixbuf> > unsizedImg;

  Darimasen(std::vector<Glib::ustring>);
  ~Darimasen();

  short get_mode(){return mode;}

  Gtk::CheckMenuItem * optShowHidden;
  void set_message(Glib::ustring);
  //std::vector< std::stack<Glib::ustring> > history;
  void updateView(Glib::ustring, Glib::ustring);

  void newTab(Glib::ustring);
  void ChangeCurrentPath(Glib::ustring path, bool, bool);
  


  Glib::ustring get_history(gint, gint = 0);

  void set_history(gint, Glib::ustring);

  bool del_history(gint);

  void buildHistoryMenu(gint);
  void fHistoryMenu(gint, gint);
  Gtk::Notebook * Tabber;
  };

/**********************/

#endif //DARIMASEN_H
