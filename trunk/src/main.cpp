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

    Darimasen window(path);
    Gtk::Main::run(window); 
    return 0;
}

/**********************/
