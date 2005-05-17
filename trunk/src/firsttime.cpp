/* Darimasen - firsttime.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

/**********************/

#include "firsttime.h"

/**********************/

firsttime::firsttime(){  

   okbutton1 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   image1 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-dialog-question"), Gtk::IconSize(6)));
   label1 = Gtk::manage(new class Gtk::Label("It looks like this is your first time\n"
		"running Darimasen (and you \n"
		"have not used ROX).\n\n"
		"This is a good time to set up\n"
		"some default \"Run Actions\"\n\n"
		"Simply replace the wildcard (*)\n"
		"with your preferred program."));
   hbox1 = Gtk::manage(new class Gtk::HBox(false, 0));
   textlabel = Gtk::manage(new class Gtk::Label("Text Files:"));
   musiclabel = Gtk::manage(new class Gtk::Label("Music Files:"));
   videolabel = Gtk::manage(new class Gtk::Label("Video Files:"));
   imagelabel = Gtk::manage(new class Gtk::Label("Image Files:"));
   textentry = Gtk::manage(new class Gtk::Entry());
   musicentry = Gtk::manage(new class Gtk::Entry());
   videoentry = Gtk::manage(new class Gtk::Entry());
   imageentry = Gtk::manage(new class Gtk::Entry());
   table1 = Gtk::manage(new class Gtk::Table(2, 2, false));
   hbox2 = Gtk::manage(new class Gtk::HBox(false, 0));
   vbox1 = Gtk::manage(new class Gtk::VBox(false, 0));
   okbutton1->set_flags(Gtk::CAN_FOCUS);
   okbutton1->set_flags(Gtk::CAN_DEFAULT);
   okbutton1->set_relief(Gtk::RELIEF_NORMAL);
   get_action_area()->property_layout_style().set_value(Gtk::BUTTONBOX_END);
   image1->set_alignment(0.5,0.5);
   image1->set_padding(6,0);
   label1->set_alignment(0.5,0.5);
   label1->set_padding(0,0);
   label1->set_justify(Gtk::JUSTIFY_LEFT);
   label1->set_line_wrap(true);
   label1->set_use_markup(false);
   label1->set_selectable(false);
   hbox1->pack_start(*image1, Gtk::PACK_SHRINK, 0);
   hbox1->pack_start(*label1);

   textlabel->set_alignment(0.96,0.5);

   textentry->set_flags(Gtk::CAN_FOCUS);
   textentry->set_text("* \"$@\"");

   musicentry->set_flags(Gtk::CAN_FOCUS);
   musicentry->set_text("* \"$@\"");

   videoentry->set_flags(Gtk::CAN_FOCUS);
   videoentry->set_text("* \"$@\"");

   imageentry->set_flags(Gtk::CAN_FOCUS);
   imageentry->set_text("* \"$@\"");

   table1->set_border_width(2);
   table1->set_row_spacings(5);
   table1->set_col_spacings(4);
   table1->attach(*textlabel, 0, 1, 0, 1, Gtk::FILL);
   table1->attach(*musiclabel, 0, 1, 1, 2, Gtk::FILL);
   table1->attach(*videolabel, 0, 1, 2, 3, Gtk::FILL);
   table1->attach(*imagelabel, 0, 1, 3, 4, Gtk::FILL);
   table1->attach(*textentry, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL);
   table1->attach(*musicentry, 1, 2, 1, 2, Gtk::EXPAND|Gtk::FILL);
   table1->attach(*videoentry, 1, 2, 2, 3, Gtk::EXPAND|Gtk::FILL);
   table1->attach(*imageentry, 1, 2, 3, 4, Gtk::EXPAND|Gtk::FILL);
   hbox2->pack_start(*table1);
   vbox1->pack_start(*hbox1);
   vbox1->pack_start(*hbox2);

   get_vbox()->pack_start(*vbox1);
   set_title("Darimasen: first run");


   add_action_widget(*okbutton1, -5);


   okbutton1->signal_clicked().connect(sigc::mem_fun(*this, &firsttime::confirm));


   show_all_children();
  }


/**********************/

void firsttime::confirm(){

  Glib::ustring choicesdir="/.choices";

  if ( textentry->get_text() != "* \"$@\""){ //don't intentionally make a broken script.
    Glib::ustring command = "#! /bin/sh\nexec " + textentry->get_text() + "\n";
    try {
      Glib::ustring exec1 = (Glib::get_home_dir() +  (Glib::ustring)(choicesdir + "/MIME-types/text"));
      Gnome::Vfs::Handle write_handle;
      write_handle.create(exec1, Gnome::Vfs::OPEN_WRITE, false, 0755);
      write_handle.seek(Gnome::Vfs::SEEK_POS_START, 0);
      GnomeVFSFileSize bytes_written = write_handle.write(command.data(), command.size());
      }
    catch(const Gnome::Vfs::exception) {}
    }

  if ( musicentry->get_text() != "* \"$@\""){ //don't intentionally make a broken script.
    Glib::ustring command = "#! /bin/sh\nexec " + musicentry->get_text() + "\n";
    try {
      Glib::ustring exec1 = (Glib::get_home_dir() +  (Glib::ustring)(choicesdir + "/MIME-types/audio"));
      Gnome::Vfs::Handle write_handle;
      write_handle.create(exec1, Gnome::Vfs::OPEN_WRITE, false, 0755);
      write_handle.seek(Gnome::Vfs::SEEK_POS_START, 0);
      GnomeVFSFileSize bytes_written = write_handle.write(command.data(), command.size());
      }
    catch(const Gnome::Vfs::exception) {}
    }

  if ( videoentry->get_text() != "* \"$@\""){ //don't intentionally make a broken script.
    Glib::ustring command = "#! /bin/sh\nexec " + videoentry->get_text() + "\n";
    try {
      Glib::ustring exec1 = (Glib::get_home_dir() +  (Glib::ustring)(choicesdir + "/MIME-types/video"));
      Gnome::Vfs::Handle write_handle;
      write_handle.create(exec1, Gnome::Vfs::OPEN_WRITE, false, 0755);
      write_handle.seek(Gnome::Vfs::SEEK_POS_START, 0);
      GnomeVFSFileSize bytes_written = write_handle.write(command.data(), command.size());
      }
    catch(const Gnome::Vfs::exception) {}
    }

  if ( imageentry->get_text() != "* \"$@\""){ //don't intentionally make a broken script.
    Glib::ustring command = "#! /bin/sh\nexec " + imageentry->get_text() + "\n";
    try {
      Glib::ustring exec1 = (Glib::get_home_dir() +  (Glib::ustring)(choicesdir + "/MIME-types/image"));
      Gnome::Vfs::Handle write_handle;
      write_handle.create(exec1, Gnome::Vfs::OPEN_WRITE, false, 0755);
      write_handle.seek(Gnome::Vfs::SEEK_POS_START, 0);
      GnomeVFSFileSize bytes_written = write_handle.write(command.data(), command.size());
      }
    catch(const Gnome::Vfs::exception) {}
    }

  hide();
  }

/**********************/
