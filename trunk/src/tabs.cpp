/* Darimasen - tabs.cpp - Copyright (C) 2005 Sudrien, GPL */

#include "tabs.h"


/**********************/

tabs::tabs(const std::vector<Glib::ustring>& input){

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

 // Gtk::EventBox * MainEventBox = new class Gtk::EventBox;


 // MainEventBox->modify_bg(Gtk::STATE_NORMAL, Gdk::Color("#ffffff"));
 // MainEventBox->show();

  //DaIconModes * foo;
  //foo = new class DaIconModes(pos, *this);


  // pos can't be the thing passed here, it changes... (segfault: 3 tabs, close 2nd then 3rd)
  //closeButton->signal_clicked().connect(
  //  sigc::bind<DaIconModes*>(sigc::mem_fun(*this, &Darimasen::removeTab), foo) );


/*
  if ( IconModeList.size() == pos ){
  //  IconModeList.push_badirectory * dir = Gtk::manage(new class directory(*this,pos))ck(foo);
  //  EventBoxList.push_back(MainEventBox);
    }
  else {
    delete IconModeList[pos];
    IconModeList[pos] = foo;
    delete EventBoxList[pos];
    EventBoxList[pos] = MainEventBox;
    }*/

 // Gtk::ScrolledWindow * MainScroller = new Gtk::ScrolledWindow;
 // MainScroller->show();
 // MainScroller->set_shadow_type(Gtk::SHADOW_NONE);
  //MainScroller->add(*MainEventBox);
  //MainEventBox->add(*foo);
  //MainScroller->set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_AUTOMATIC);
  //foo->show();

 // Gtk::EventBox * MainEventBox = new class Gtk::EventBox;


 // MainEventBox->modify_bg(Gtk::STATE_NORMAL, Gdk::Color("#ffffff"));

  //directory * dir = Gtk::manage(new class directory(*this,pos))
  directory * dir = Gtk::manage(new class directory());
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

    Glib::ustring ico;
  try{
    ico = "/usr/share/icons/gnome/48x48/mimetypes/gnome-mime-";
    ico += mime.replace(mime.find("/"), 1, "-");
    ico += ".png";

    Glib::file_get_contents(ico);
    }
  catch(const Glib::Error) {
    try{
      ico = "/usr/share/icons/gnome/48x48/mimetypes/gnome-mime-";
      ico += mime.substr(0,mime.find("-"));
      ico += ".png";
      Glib::file_get_contents(ico);
      }
    catch(const Glib::Error) {
     ico = "/usr/share/icons/gnome/48x48/filesystems/gnome-fs-loading-icon.png";
     }
    }

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
