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
//#include <gdkmm/rectangle.h>
/**********************/

class Darimasen : public Gtk::Window {
  class DarimasenMenu : public Gtk::MenuBar {
    class DirectoryMenu : public Gtk::Menu {
    public:

      bool imExtended;
      DirectoryMenu(DarimasenMenu&){
imExtended = false;
        Gtk::Menu::Menu();
        }
      DarimasenMenu * parent; 

      void on_hide(){

      std::cout << "on_hide()\n";
    //  if (imExtended)
      //   parent->offClick();
        Gtk::Widget::on_hide();
     }

/*
      void on_size_allocate(Gtk::Allocation&  allocation){
        std::cout << allocation.get_x() << ", " << allocation.get_y() << ", " << allocation.get_width() << ", " << allocation.get_height() << "\n";
allocation.set_x (allocation.get_x() - 20);
        Gtk::Widget::on_size_allocate(allocation);
        }
void on_size_request (Requisition* requisition){
        std::cout << "Req = " << &requisition.x() << ", " << requisition.y() << ", " << requisition.width() << ", " << requisition.height() << "\n";
}*/

    };

    Darimasen * parent;

    int depth;

     DirectoryMenu ** MenuArray; 
    Gtk::MenuItem ** MenuItemArray;
    Glib::ustring * menulevel;

    void MenuForPath(int, Glib::ustring , Glib::ustring); //extension
    void SpecialMenuForPath(int, Glib::ustring  , Glib::ustring); //extension

    void offClick();

    Glib::ustring CountSubdir(const Glib::ustring&);
    bool DaMenuSelect(GdkEventButton*, const Glib::ustring,guint);
 

 //    void on_popup_menu_position(int& x, int& y, bool& push_in);





  public:
short extended;
    void signal_deactivate ();
    DarimasenMenu(const Glib::ustring&, Darimasen&, guint);
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



  Gtk::ToolButton * BackButton;

  void fNewTab();
  void newTab(Glib::ustring);
  void tabberSwitched(GtkNotebookPage*, guint);
  void addTab(guint);
  void ChangeCurrentPath(Glib::ustring path, bool);
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
  std::vector< std::stack<Glib::ustring> > history;
  };

/**********************/

#endif //DARIMASEN_H




