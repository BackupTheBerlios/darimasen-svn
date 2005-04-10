/* Darimasen - firsttime.h - Copyright (C) 2004 - 2005 Sudrien, GPL */

#ifndef FIRSTTIME_H
#define FIRSTTIME_H

#include "main.h"
#include <gtkmm/dialog.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>

/**********************/


class dialog1_glade : public Gtk::Dialog
{  


        class Gtk::Button * okbutton1;
        class Gtk::Image * image1;
        class Gtk::Label * label1;
        class Gtk::HBox * hbox1;
        class Gtk::Label * label2;
        class Gtk::Label * label3;
        class Gtk::Label * label4;
        class Gtk::Label * label5;
        class Gtk::Entry * entry1;
        class Gtk::Entry * entry2;
        class Gtk::Entry * entry3;
        class Gtk::Entry * entry4;
        class Gtk::Table * table1;
        class Gtk::HBox * hbox2;
        class Gtk::VBox * vbox1;

        void confirm();
      
public:
        
        dialog1_glade();
};

/**********************/

#endif //FIRSTTIME_H
