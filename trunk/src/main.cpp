/* Darimasen - main.cpp - Copyright (C) 2005 Sudrien, GPL */
   
#include "main.h"



int main(int argc, char *argv[]){

  Gtk::Main kit(argc, argv);
  Gnome::Vfs::init();


  // next blocks are to check the existance of required directories.
  // make .choices if nothing available
  if (!Gnome::Vfs::Uri::create(Glib::get_home_dir() + get_choices_dir())->uri_exists()){
    gnome_vfs_make_directory((Glib::get_home_dir() + get_choices_dir()).c_str(),493);
    }

  // make MIME-types if nothing available, set up basics
  if (!Gnome::Vfs::Uri::create(Glib::get_home_dir() + get_choices_dir() + "/MIME-types")->uri_exists()){
    gnome_vfs_make_directory((Glib::get_home_dir() + get_choices_dir() + "/MIME-types").c_str(),493);
    //firsttime window; 
    //Gtk::Main::run(window);
    }

  // make our own settings folder if nothing available
  if (!Gnome::Vfs::Uri::create(Glib::get_home_dir() + get_choices_dir() + "/Darimasen")->uri_exists()){
    gnome_vfs_make_directory((Glib::get_home_dir() + get_choices_dir() + "/Darimasen").c_str(),493);
    }
  

  std::vector<Glib::ustring> path;

  if(path.empty())
    path.push_back(Glib::get_home_dir() + "/");


  darimasen_window window(path);
  Gtk::Main::run(window);


  return 0;
  }
