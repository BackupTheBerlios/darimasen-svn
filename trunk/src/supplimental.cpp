/* Darimasen - supplimental.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

/**********************/

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
  if (Gnome::Vfs::Uri::create(Glib::get_home_dir() + "/Choices")->uri_exists()) return "/Choices";
  else return "/.choices";
  }

/**********************/

//execute a command in the correct working directory
void goruncommand(Glib::ustring exec, Glib::ustring path){
    Glib::spawn_async(path, Glib::shell_parse_argv(exec), Glib::SPAWN_SEARCH_PATH);
  }
  
/**********************/

Glib::ustring trim_whitespace(const Glib::ustring& text)
{
  Glib::ustring::const_iterator pbegin (text.begin());
  Glib::ustring::const_iterator pend   (text.end());

  while(pbegin != pend && Glib::Unicode::isspace(*pbegin))
    ++pbegin;

  Glib::ustring::const_iterator temp (pend);

  while(pbegin != temp && Glib::Unicode::isspace(*--temp))
    pend = temp;

  return Glib::ustring(pbegin, pend);
}

/**********************/ 
