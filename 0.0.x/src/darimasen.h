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

#include "main.h"
#include <sys/types.h>
#include <gtkmm/menutoolbutton.h>

#include "optionsManager.h"
#include "damenu.h"
#include "iconmodes.h"

/**********************/

class Darimasen : public Gtk::Window {

public:

  Darimasen(std::vector<Glib::ustring>);
  ~Darimasen();

private:

  Glib::RefPtr<Gdk::Pixbuf> windowIcon;
  short mode;


  Gtk::VBox VerticalOrganizer;
  Gtk::Toolbar TopBar;

  Gtk::ToolItem * CompactMenuContainer;
  Gtk::MenuBar * CompactMenu;
  Gtk::Menu * m_Menu_File;
  Gtk::CheckMenuItem * optShowHidden;

  Gtk::SeparatorToolItem * sep1;

  Gtk::ToolItem * DarimasenMenuContainer;
  class DarimasenMenu * DaMenu;

  Gtk::SeparatorToolItem * sep2;

  Gtk::MenuToolButton * BackButton;
  Gtk::Menu * history_menu;
  Gtk::ToolButton * ChangeIconMode;
  //Gtk::ToggleToolButton * ViewTree;
  Gtk::ToolButton * NewTab;

  Gtk::HPaned HideTreePane;
  Gtk::ScrolledWindow TreeScroller;
  Gtk::Notebook * Tabber;

  Gtk::Statusbar Info;


  std::vector< class DaIconModes* > IconModeList;
  std::vector< class Gtk::EventBox* > EventBoxList;
  std::vector< std::vector<Glib::ustring> > history;
  std::vector <Glib::ustring> mimeList;
  std::vector <Glib::RefPtr<Gdk::Pixbuf> > unsizedImg;

  class optionsQuery * myOptions;

Gtk::Window * about;

  void addTab(guint);
  void fAbout();
  bool fAbout_ch0wned(GdkEventButton*);
  void fBack();
  void fChangeIconMode();
  void fNewTab();
  void fPrintHist();
  void fQuit();
  void fShowHidden();
  void removeTab(DaIconModes*&);
  void tabberSwitched(GtkNotebookPage*, guint);


public:


  void buildHistoryMenu(gint);
  //rework so the bools mean somthing consistant
  void ChangeCurrentPath(Glib::ustring path, bool, bool);
  bool del_history(gint);
  bool get_hidden();
  Glib::ustring get_history(gint, gint = 0);
  Glib::RefPtr<Gdk::Pixbuf> get_icon(Glib::ustring mime);
  short get_mode();
  void fHistoryMenu(gint, gint);
  void newTab(Glib::ustring);
  void set_message(Glib::ustring);
  void set_history(gint, Glib::ustring);
  void updateView(Glib::ustring, Glib::ustring);

  };

/**********************/

#endif //DARIMASEN_H
