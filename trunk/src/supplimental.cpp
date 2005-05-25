
#include "supplimental.h"

/**********************/

//check which choices dir to use
Glib::ustring get_choices_dir(){
  if (Gnome::Vfs::Uri::create(Glib::get_home_dir() + "/Choices")->uri_exists()) return "/Choices";
  else return "/.choices";
  }
