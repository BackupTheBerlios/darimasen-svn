/* Darimasen - main.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */
 

#include "main.h"

/**********************/

int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);
  
  Glib::ustring path = ""; //Testing if directory is given in argument list
  for( int i=1; i < argc; i++){
    if((argv[i][0]!='-') && (argv[i-1][0]!='-')){ // some quick anti-argument logic
      path = argv[i];
      }
    }

  if(path == "")
    #ifdef WIN32
    path = getenv("USERPROFILE");
    #else
    path = getenv("HOME");
    #endif

    if(path.substr(path.length()-1,1) != slash )
      path += slash;

    // What should be sent to the constructor is a correctly formed, existant path.
    // This should be checked better in the future.
    Darimasen window(path);
    Gtk::Main::run(window); 
    return 0;
}

/**********************/
