/* Darimasen - iconmodes.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

#include "iconmodes.h"
#include <iostream>

/**********************/

DaIconModes::DaIconModes(Glib::ustring path, int atPath, unsigned short usingMode, int MainScrollerHeight, bool givehidden){
                                  
  Table();
    
  fullPath = path;
  filesAtPath = atPath;
  iconmode = usingMode;
  heightAvailable = MainScrollerHeight;
  showHidden = givehidden;
    
  
  switch(iconmode){  
    case 0: DaSideconLister(); break;
    case 1: DaDetailLister(); break;
    } 
  }

/**********************/

void DaIconModes::DaSideconLister(){
             
  int y = heightAvailable / 58; //number 58 is based of pango metrics, I think.
  int x = filesAtPath / y + 1;
  
  resize ( y ,x );
  double_buffered();
  DIR *d;
  struct dirent *dir;
  d = opendir(fullPath.c_str());
  Glib::ustring file;
  int i = 0;

  if (d){
    while (dir = readdir(d)){
      Glib::ustring tmp3 = dir->d_name;
      if( (tmp3.find(".") == 0) xor (showHidden == false) ){
        Sidecon * tempPath;
        if ( chdir( (fullPath + slash + tmp3).c_str()) == -1 ){
          //there is probably come memory loss in the next line...
          //where should the "delete tempPath;" code go?
          tempPath = new Sidecon(fullPath  + tmp3);
          attach(*tempPath, i/y, i/y+1, i%y, i%y+1, Gtk::FILL, Gtk::FILL, 4, 4);
          tempPath->show();
          i++;
          }
        }
      }
    closedir(d);
    }

    show();

  }

/**********************/

void DaIconModes::DaDetailLister(){
             
  resize ( (filesAtPath > 1) ? filesAtPath : 1 ,1 );

  DIR *d;
  struct dirent *dir;
  d = opendir(fullPath.c_str());
  Glib::ustring file;
  int i = 0;
  if (d){
    while (dir = readdir(d)){
      Glib::ustring tmp3 = dir->d_name;
      Gtk::Label * tempPath;
      if ( chdir( (fullPath + slash + tmp3).c_str()) == -1 ){
        // this will be more than a label by 0.0.6 final
        tempPath = Gtk::manage(new Gtk::Label(tmp3));
        tempPath->set_alignment(0,0.5);
        attach(*tempPath, 0, 1, i, i+1, Gtk::FILL, Gtk::FILL, 4, 4);
        tempPath->show();
        i++;
        }
      }
    closedir(d);
    }
  show();
  }

/**********************/
