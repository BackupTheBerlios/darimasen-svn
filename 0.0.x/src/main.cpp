/* Darimasen - main.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */
   
/**********************/
  
#include "main.h"

#include "firsttime.h"
#include "darimasen.h"

/**********************/
  
int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);
  Gnome::Vfs::init();

  //The new get_choices_dir() really speeds this up...
  //Glib::get_home_dir() + get_choices_dir(); -- will be ~/Choices if exists, ~/.choices otherwise
  {
    // make .choices if nothing available
    if (!Gnome::Vfs::Uri::create(Glib::get_home_dir() + get_choices_dir())->uri_exists()){
      gnome_vfs_make_directory((Glib::get_home_dir() + get_choices_dir()).c_str(),493);
      }

    // make MIME-types if nothing available, set up basics
    if (!Gnome::Vfs::Uri::create(Glib::get_home_dir() + get_choices_dir() + "/MIME-types")->uri_exists()){
      gnome_vfs_make_directory((Glib::get_home_dir() + get_choices_dir() + "/MIME-types").c_str(),493);
      firsttime window; 
      Gtk::Main::run(window);
      }

    // make our own settings folder if nothing available
    if (!Gnome::Vfs::Uri::create(Glib::get_home_dir() + get_choices_dir() + "/Darimasen")->uri_exists()){
      gnome_vfs_make_directory((Glib::get_home_dir() + get_choices_dir() + "/Darimasen").c_str(),493);
      }
    }

  std::vector<Glib::ustring> path;

  // parse all the possible paths on the command line
  for( int i=1; i < argc; i++){
    if((argv[i][0]!='-') && (argv[i-1][0]!='-')){ // some quick anti-argument logic
      Glib::ustring tmp = argv[i];
      if(tmp.substr(tmp.length()-1,1) != slash )
        tmp += slash;
       Glib::RefPtr<Gnome::Vfs::Uri> x = Gnome::Vfs::Uri::create(tmp);
      // case that URL is Absolute, and works:
      if (x->uri_exists()){
        path.push_back(x->get_path());
        }
      // case that URL is Absolute, but pointing to a file
      else if (tmp.substr(0,1) == slash){
        x = x->get_parent();
        if (x->uri_exists()){
          path.push_back(x->get_path()+slash);
          }
        }
      // URL can only be relative. 
      else {
        x = x->resolve_relative (get_current_dir_name() + slash + tmp);
        //URL was relative, and works.
        if (x->uri_exists()){
          path.push_back(x->get_path());
          }
        // URL is relative, but pointing to file
        else{
            x = x->get_parent();
            path.push_back(x->get_path()+slash);
          }
        }
      }
    }

  if(path.empty())
    path.push_back(Glib::get_home_dir() + slash);


  std::cout << "Tabs opened should be for:\n";
  for(int i =0; i< path.size();i++)
      std::cout << path[i] <<  "\n";

  Darimasen window(path);
  Gtk::Main::run(window); 

  return 0;
  }


/**********************/
