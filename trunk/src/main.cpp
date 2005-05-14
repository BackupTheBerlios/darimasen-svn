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

  // check for mime definitions. 
  { 
    Glib::ustring fuz = Glib::get_home_dir() ;
    Glib::ustring altfuz = fuz;
    fuz += "/Choices";
    Glib::RefPtr<Gnome::Vfs::Uri> fuzz = Gnome::Vfs::Uri::create(fuz);
    //support ./choices as well
    altfuz += "/.choices";
    Glib::RefPtr<Gnome::Vfs::Uri> altfuzz = Gnome::Vfs::Uri::create(altfuz);  

    //make .choices default - if people don't have Rox they'd
    // probably prefer not to have their homedir cluttered  
    if (!fuzz->uri_exists() && !altfuzz->uri_exists()){
      gnome_vfs_make_directory(altfuz.c_str(),493);
      }

    //check which choices dir to use, use .choices if Choices isn't there      
    if (!fuzz->uri_exists()){
   	fuz = altfuz;
    }

   // make actions folder if needed
   Glib::RefPtr<Gnome::Vfs::Uri> fuz2 = Gnome::Vfs::Uri::create(fuz + "/MIME-types");
    if (!fuz2->uri_exists()){
      gnome_vfs_make_directory((fuz + "/MIME-types").c_str(),493);
      dialog1_glade window; 
      Gtk::Main::run(window); 
      }
    

   // make our own settings folder if needed
   Glib::RefPtr<Gnome::Vfs::Uri> fuz3 = Gnome::Vfs::Uri::create(fuz + "/Darimasen");
    if (!fuz3->uri_exists()){
      gnome_vfs_make_directory((fuz + "/Darimasen").c_str(),493);
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
    path.push_back(Glib::get_home_dir() + slash);


  std::cout << "Tabs opened should be for:\n";
  for(int i =0; i< path.size();i++)
      std::cout << path[i] <<  "\n";

  Darimasen window(path);
  Gtk::Main::run(window); 

  return 0;
  }


/**********************/
