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
