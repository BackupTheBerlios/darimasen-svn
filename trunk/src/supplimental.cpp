/* Darimasen - supplimental.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

#include "supplimental.h"

/**********************/

Glib::ustring int2ustr(int x){
  Glib::ustring tmp ="";
  char tmp2 =' ';
  
  do{
    tmp2 = static_cast<char>((x % 10)+48);
    tmp = tmp2 + tmp;
    x = static_cast<int>(x/10.0);
    }while (x > 0);          
  return tmp;
  }

/**********************/

Glib::ustring underscoreSafe(Glib::ustring x){
  int startAtPos = 0, found = 0;
  Glib::ustring tmp = x;

  while (x.find("_",startAtPos) != -1){
          startAtPos = x.find("_", startAtPos);
          tmp.replace(startAtPos++ + found++, 1, "__");  
          }
   
  return tmp;
  }

/**********************/

//check which choices dir to use
Glib::ustring getchoicesdir(){
  Glib::RefPtr<Gnome::Vfs::Uri> choicespath = Gnome::Vfs::Uri::create(
    (Glib::ustring)getenv("HOME") + "/Choices");
  if (choicespath->uri_exists()){
    return "/Choices";
    }
  return "/.choices";
  }

/**********************/
