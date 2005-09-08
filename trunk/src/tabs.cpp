/* Darimasen - tabs.cpp - Copyright (C) 2005 Sudrien, GPL */

#include "tabs.h"


/**********************/

tabs::tabs(class darimasen_window& ancestor, const std::vector<Glib::ustring>& input){

  //set_tab_pos(Gtk::POS_BOTTOM);

  // begin the history  
  for(gint i = 0; i < input.size(); i++){
    set_history(i, input[i]);
    add_tab(i);
    }
  show_all_children();
  show();
  }

/**********************/

tabs::~tabs(){}

/**********************/

void tabs::add_tab(gint pos){


 // if( get_n_pages() == 0 )
 //   set_show_tabs(false);
 // else
    set_show_tabs(true);


  Gtk::Image * xed = Gtk::manage(
    new Gtk::Image("/usr/share/icons/hicolor/16x16/stock/generic/stock_close.png"));
  xed->show();

  Gtk::Label * tabNum;

  if ( get_history(pos) == "/")
    tabNum = Gtk::manage(new Gtk::Label("/ "));
  else if ( get_history(pos) == (Glib::get_home_dir() + "/"))
    tabNum = Gtk::manage(new Gtk::Label("~/ "));
  else
    tabNum = Gtk::manage(new Gtk::Label(
      get_history(pos).substr(get_history(pos).rfind("/",get_history(pos).length() - 2  ) + 1)));


  Gtk::HBox * arrangement= Gtk::manage(new Gtk::HBox()) ;
  Gtk::Button * closeButton = Gtk::manage(new Gtk::Button());

  closeButton->add(*xed);
  closeButton->set_relief(Gtk::RELIEF_NONE); 
  closeButton->show();

  tabNum->show();
  arrangement->show();

  arrangement->pack_start(*tabNum);
  arrangement->pack_end(*closeButton);

  directory * dir = Gtk::manage(new class directory(*this,pos));
  insert_page( *dir, *arrangement, pos);

  }

/**********************/

Glib::RefPtr<Gdk::Pixbuf> tabs::get_icon(Glib::ustring mime){
  int i;

  for(i = 0; i< mimeList.size(); i++){
    if( mime == mimeList[i] ){
      return unsizedImg[i];
      }
    }
   // pic doesn't exist.

    Glib::ustring ico = "/usr/share/icons/gnome/48x48/mimetypes/gnome-mime-";
Glib::ustring t;
  try{
    t = mime.replace(mime.find("/"), 1, "-");
    t += ".png";

    Glib::file_get_contents(ico + t);
    }
  catch(const Glib::Error) {
    try{
      t = mime.substr(0,mime.find("-"));
      t += ".png";
      Glib::file_get_contents(ico + t);
      }
    catch(const Glib::Error) {
     ico = "/usr/share/icons/gnome/48x48/filesystems/gnome-fs-loading-icon.png";
     t = "";
     }
    }
ico += t;

  mimeList.push_back(mime);
  Glib::RefPtr<Gdk::Pixbuf> xe = Gdk::Pixbuf::create_from_file(ico);
  unsizedImg.push_back(xe);
  return unsizedImg[i]; 
  }

/**********************/

Glib::ustring tabs::get_history(gint tab, gint level) const{
  return history[tab][history[tab].size()-level-1];
  }

/**********************/

void tabs::set_history(gint tab, Glib::ustring path){
  if (get_n_pages() == tab){
    std::vector<Glib::ustring> empty;
    history.push_back(empty);
    history[tab].push_back(path);
    }
  else {
    history[tab].push_back(path);
    }
  }

/**********************/

gint tabs::rm_history(gint tab){
  history[tab].pop_back();
  if ( history[tab].size() == 0 ){
    history.erase(history.begin()+tab,history.begin()+tab+1 );
    return 0;
    }
  return history[tab].size();
  }

/**********************/
