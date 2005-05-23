/* Darimasen - optionsManager.h - Copyright (C) 2004 - 2005 Sudrien, GPL */



#ifndef OPTIONS_H
#define OPTIONS_H

/**********************/

#include "main.h"

/**********************/

class optionsQuery{
public:

  optionsQuery(Glib::ustring);
  ~optionsQuery();

private:


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

  Glib::ustring get_option(Glib::ustring);
  bool set_option(Glib::ustring, Glib::ustring);
  };

/**********************/

#endif //SUPPLIMENTAL_H
