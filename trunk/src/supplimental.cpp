
#include "supplimental.h"

/**********************/

//check which choices dir to use
Glib::ustring get_choices_dir(){
  if (Gnome::Vfs::Uri::create(Glib::get_home_dir() + "/Choices")->uri_exists()) return "/Choices";
  else return "/.choices";
  }

Glib::ustring int2ustr(gint x){
  Glib::ustring tmp ="";
  char tmp2 =' ';
  
  do{
    tmp2 = static_cast<char>((x % 10)+48);
    tmp = tmp2 + tmp;
    x = static_cast<int>(x/10.0);
    }while (x > 0);          
  return tmp;
  }
