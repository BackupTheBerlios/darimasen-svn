/* Darimasen - firsttime.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef FIRSTTIME_H
#define FIRSTTIME_H

#include "main.h"

/**********************/

class firsttime : public Gtk::Dialog{  

public:
        
  firsttime();

private:

  Gtk::Button * okbutton1;
  Gtk::Image * image1;
  Gtk::Label * label1;
  Gtk::HBox * hbox1;
  Gtk::Label * textlabel;
  Gtk::Label * imagelabel;
  Gtk::Label * videolabel;
  Gtk::Label * musiclabel;
  Gtk::Entry * textentry;
  Gtk::Entry * imageentry;
  Gtk::Entry * videoentry;
  Gtk::Entry * musicentry;
  Gtk::Table * table1;
  Gtk::HBox * hbox2;
  Gtk::VBox * vbox1;

  void confirm();
  };


/**********************/

#endif //FIRSTTIME_H
