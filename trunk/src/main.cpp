/* Darimasen - main.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */
 

#include "main.h"

/**********************/

int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);
  Gnome::Vfs::init();

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
          try{
            Glib::file_get_contents(x->get_path().substr(0,x->get_path().length()-1) );
            x = x->get_parent();
            path.push_back(x->get_path()+slash);
            }
          catch(const Glib::Error) {/*non-existant file OR folder*/}
          }
        }
      }
    }

  if(path.empty())
#ifdef WIN32
  path.push_back(getenv("USERPROFILE") + slash));
#else
  path.push_back(getenv("HOME") + slash);
#endif

  std::cout << "Tabs opened should be for:\n";
  for(int i =0; i<path.size();i++)
      std::cout << path[i]
                << /*" " 
                << path[i].substr(path[i].rfind(slash, path[i].length() -2)+1)
                <<*/ "\n";

  Darimasen window(path);
  Gtk::Main::run(window); 
  return 0;
}

/**********************/
