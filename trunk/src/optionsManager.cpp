/* Darimasen - optionsManager.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

/**********************/

#include "optionsManager.h"

/**********************/

void optionsQuery::DumpParser::on_start_element(Glib::Markup::ParseContext&,
                                  const Glib::ustring&,
                                  const AttributeMap&  attributes){
  for(AttributeMap::const_iterator p = attributes.begin(); p != attributes.end(); ++p){
    std::cout <<  p->second ;
    }
  }

/**********************/

void optionsQuery::DumpParser::on_text(Glib::Markup::ParseContext&, const Glib::ustring& text){
  Glib::ustring trimmed_text = trim_whitespace(text);
  if (!trimmed_text.empty()) std::cout << " " << trimmed_text << "\n";
  }

/**********************/ 

// get the contents out of one of our XML settings files
optionsQuery::optionsQuery(Glib::ustring toParse){

  DumpParser parser;
  Glib::Markup::ParseContext context (parser);

  try{
    context.parse(Glib::file_get_contents(toParse));
    context.end_parse();
    }
  catch(const Glib::Error) {
    std::cout<< "Error! Settings file " << toParse << " not found.\n";
    }
  }

/**********************/ 

// write out to file
optionsQuery::~optionsQuery(){
  }

/**********************/ 

//set values in out vectors
  bool optionsQuery::set_option(Glib::ustring, Glib::ustring){}

/**********************/ 

//return set values
  Glib::ustring optionsQuery::get_option(Glib::ustring){}

/**********************/ 
