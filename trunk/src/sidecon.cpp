/* Darimasen - sidecon.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

#include "sidecon.h"
#include <iostream>


/**********************/

Sidecon::Sidecon(Glib::ustring path){
  filePath = path;
  Glib::ustring nom = filePath.substr( filePath.rfind(slash) + 1, filePath.length() - filePath.rfind(slash));
  Glib::ustring shortnom = nom;

  if (nom.length() > 25){

    int last = nom.rfind(".");

    if (last != -1){ //fix for the missing extension bug
      //std::cout << nom << "\n";
      Glib::ustring ext = nom.substr(last);  
      shortnom = nom.substr(0, 20) + ".." + ext;
      }
    else {
      shortnom = nom.substr(0,22) + "...";
      }

    }
                   
    
  
  Gtk::Table * IconArangementTable = Gtk::manage(new class Gtk::Table(3,2));
  Gtk::Image * image1 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-dnd"), Gtk::IconSize(6)));
  Gtk::Label * FileName = Gtk::manage(new class Gtk::Label(shortnom));

  // this is where the mimetype info goes
  Gtk::Label * FilePermissions = Gtk::manage( new class Gtk::Label("--"));


  struct stat buf;
  stat(path.c_str(), &buf);

  // THis is simply file size - images an video could have more. Much later.
  Glib::ustring size;
  if( buf.st_size < 1024 ){
    size = int2ustr(buf.st_size) + " B";
    }
  else if ( buf.st_size < (1024 * 1024) ){
    size = int2ustr(buf.st_size / 1024) + " KB";
    }
  else{
    size = int2ustr(buf.st_size / 1024 / 1024) + " MB";
    }

  // ideally, this label would be 80% size of standard
  Gtk::Label * FileSizeInfo = Gtk::manage( new class Gtk::Label(size));

  FileName->set_alignment(0,0.5);
  FileName->set_justify(Gtk::JUSTIFY_LEFT);
  FilePermissions->set_alignment(0,0.5);
  FilePermissions->set_justify(Gtk::JUSTIFY_LEFT);
  FileSizeInfo->set_alignment(0,0.5);
  FileSizeInfo->set_justify(Gtk::JUSTIFY_LEFT);
  
  IconArangementTable->attach(*image1, 0, 1, 0, 3, Gtk::FILL, Gtk::FILL, 0, 0);
  IconArangementTable->attach(*FileName, 1, 2, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
  IconArangementTable->attach(*FilePermissions, 1, 2, 1, 2, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
  IconArangementTable->attach(*FileSizeInfo, 1, 2, 2, 3, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
  
  set_visible_window(false);
  add(*IconArangementTable);
  image1->show();
  FileName->show();
  FilePermissions->show();
  FileSizeInfo->show();
  IconArangementTable->show();
  
  set_events(Gdk::BUTTON_RELEASE_MASK);
  signal_button_press_event().connect(
    sigc::mem_fun(*this, &Sidecon::on_eventbox_button_press) );
    
  
  Gtk::Menu::MenuList& menulist = m_Menu_Popup.items();
  
    // following is needed so underscores show correctly
    Gtk::MenuItem * op = Gtk::manage( new Gtk::MenuItem("Open " + nom));
    op->show();
    menulist.push_back( Gtk::Menu_Helpers::MenuElem(*op));
  menulist.push_back( Gtk::Menu_Helpers::MenuElem("Rename"));
  menulist.push_back( Gtk::Menu_Helpers::MenuElem("Delete "));
  menulist.push_back( Gtk::Menu_Helpers::SeparatorElem());
  menulist.push_back( Gtk::Menu_Helpers::MenuElem("Copy"));
  menulist.push_back( Gtk::Menu_Helpers::MenuElem("Move"));
  menulist.push_back( Gtk::Menu_Helpers::MenuElem("Link"));
  menulist.push_back( Gtk::Menu_Helpers::SeparatorElem());
  menulist.push_back( Gtk::Menu_Helpers::MenuElem("Properties... "));
  
  
  //sigc::mem_fun(*this, &ExampleWindow::on_menu_file_popup_generic) ) );
  //  menulist.push_back( Gtk::Menu_Helpers::MenuElem("_Process", Gtk::AccelKey("<control>p"),
  //    sigc::mem_fun(*this, &ExampleWindow::on_menu_file_popup_generic) ) );
  //  menulist.push_back( Gtk::Menu_Helpers::MenuElem("_Remove",
  //    sigc::mem_fun(*this, &ExampleWindow::on_menu_file_popup_generic) ) );

    //Add a ImageMenuElem:
  //  menulist.push_back( Gtk::Menu_Helpers::ImageMenuElem("_Something", m_Image,
  //    sigc::mem_fun(*this, &ExampleWindow::on_menu_file_popup_generic) ) ) ;
    m_Menu_Popup.accelerate(*this);
      show_all_children();
  }

/**********************/

Sidecon::~Sidecon(){
  }
  
/**********************/

// make a generic icon action
bool Sidecon::on_eventbox_button_press(GdkEventButton* event){
  if ((event->type == GDK_2BUTTON_PRESS) && (event->button == 1)){

#ifdef WIN32
    if ( (int)(ShellExecute( NULL, "Open", filePath.c_str(), "", getenv("USERPROFILE"),SW_SHOWNORMAL)) > 32)
      return true;
    // send some sort of message to the status bar that no app is associated
    // should be in statusbar 
    //std::cerr << "Could not open file, probably no program associated.\n";                  
    return false;
#else
    const std::string& openString = "gnome-open ";
    const std::string& pathString = filePath.c_str();
    const std::string& commandString = openString + filePath.c_str();
    Glib::spawn_command_line_async(commandString);
    return true;
#endif
    }

  if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3)){
    m_Menu_Popup.popup(event->button, event->time);
    return true;


    }

  return false;
  }

/**********************/
