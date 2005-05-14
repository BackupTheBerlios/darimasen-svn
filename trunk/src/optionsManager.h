/* Darimasen - optionsManager.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef OPTIONS_H
#define OPTIONS_H

/**********************/

#include "main.h"

/**********************/

Glib::ustring int2ustr(int);
Glib::ustring underscoreSafe(Glib::ustring);
Glib::ustring getchoicesdir();
void goruncommand(Glib::ustring, Glib::ustring);
Glib::ustring trim_whitespace(const Glib::ustring&);
/**********************/

class optionsQuery{


  class DumpParser : public Glib::Markup::Parser {
    void on_start_element(Glib::Markup::ParseContext&, const Glib::ustring&, const AttributeMap&);
    void on_text(Glib::Markup::ParseContext& context, const Glib::ustring& text);
    };

  struct optionholder {
    Glib::ustring name;
    Glib::ustring value;
    };

  std::vector<optionholder> optionVector;
  
  public:

  optionsQuery(Glib::ustring);
  ~optionsQuery();

  bool set_option(Glib::ustring, Glib::ustring);

  Glib::ustring get_option(Glib::ustring);
  };

/**********************/

#endif //SUPPLIMENTAL_H
