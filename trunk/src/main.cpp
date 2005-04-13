/* Darimasen - main.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */
 

#include "main.h"
#include "firsttime.h"

/**********************/

int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);
  Gnome::Vfs::init();

  // check for mime definitions. 
  { 
    Glib::ustring fuz = getenv("HOME") ;
    fuz += "/Choices";
    Glib::RefPtr<Gnome::Vfs::Uri> fuzz = Gnome::Vfs::Uri::create(fuz);
    if (!fuzz->uri_exists()){

  gnome_vfs_make_directory(fuz.c_str(),493);
  
      }



   fuz += "/MIME-types";
   Glib::RefPtr<Gnome::Vfs::Uri> fuz2 = Gnome::Vfs::Uri::create(fuz);
    if (!fuz2->uri_exists()){

  gnome_vfs_make_directory(fuz.c_str(),493);
    dialog1_glade window;
  Gtk::Main::run(window); 

      }

  }



  std::vector<Glib::ustring> path;

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
#ifdef WIN32
  path.push_back(getenv("USERPROFILE") + slash);
#else
  path.push_back(getenv("HOME") + slash);
#endif

  std::cout << "Tabs opened should be for:\n";
  for(int i =0; i< path.size();i++)
      std::cout << path[i] <<  "\n";

  Darimasen * window = new Darimasen(path);
  Gtk::Main::run(*window); 

delete window;
  return 0;
}

/**********************/
