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
#include <gtkmm/arrow.h>
#include "iconmodes.h"
#include "main.h"
#include <sys/types.h>
#include <gtkmm/eventbox.h>

#include <gtkmm/menuitem.h>
#include <libgnomevfsmm.h>

#include "supplimental.h"
#include <iostream>
//#include <gdkmm/window.h>
#include <gtkmm/scrollbar.h>
#include "dirtree.h"

#ifdef WIN32
#include <windows.h>
#include <direct.h>
#endif

/**********************/

class Darimasen : public Gtk::Window
{
public:
  Darimasen(Glib::ustring);
  virtual ~Darimasen();
    
protected:

  int depth;
  bool showHidden;
          

  Glib::ustring fullPath;
  Glib::ustring pathTruncator; //$HOME and equivelent
  std::stack<Glib::ustring> history;
  int filesAtPath;
  Gdk::Color m_Color;
  unsigned short iconmode;

     
  //Signal handlers:
  void on_menu_file_quit();
  void fBackButton();
  void fChangeIconMode();
  void fViewTree();
  void fShowHidden();
  void fActiveCompaction();  
  int resolvePath(Glib::ustring);
  int changePath(Glib::ustring, bool);
  void DaMenuBuilder(const int);
  int submenuCount(Glib::ustring);
  void DaMenuSelect(Glib::ustring);
  void iconBuild();

void DaMenu_size_allocate(Gtk::Allocation&);
void Menubar_size_allocate(Gtk::Allocation& );
  
  //bool on_configure_event(GdkEventConfigure*); // overwrite the virtual

  int MainScrollerHeight;
  int DaMenuAvailableWidth;
  int DaMenuRequestWidth;
  bool DoSomethingWithDaMenu;
  
  //Child widgets:
  Gtk::VBox m_Box;
    Gtk::Toolbar m_Controls;
      Gtk::ToolItem tb0;
        Gtk::MenuBar CompactMenu;
        Gtk::CheckMenuItem * activeCompact; 
        Gtk::CheckMenuItem * ShowHidden;
      Gtk::SeparatorToolItem separatortoolitem1;
      Gtk::ToolItem tb1;
        Gtk::MenuBar DaMenu; //the generated one ^_^
      Gtk::SeparatorToolItem separatortoolitem2;
      Gtk::ToolButton * BackButton;
      Gtk::ToolButton * ChangeIconMode;
      Gtk::ToggleToolButton * ViewTree;
    Gtk::HPaned m_hpane;
      Gtk::ScrolledWindow FileTreeScroller;

      Gtk::ScrolledWindow MainScroller;

        Gtk::EventBox * MainEventBox;   
    Gtk::Statusbar m_Statusbar;
};

/**********************/

#endif //DARIMASEN_H
