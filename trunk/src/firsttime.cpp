/* Darimasen - firsttime.cpp - Copyright (C) 2004 - 2005 Sudrien, GPL */

#include "firsttime.h"

/**********************/

dialog1_glade::dialog1_glade(
)
{  
   okbutton1 = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   image1 = Gtk::manage(new class Gtk::Image(Gtk::StockID("gtk-dialog-question"), Gtk::IconSize(6)));
   label1 = Gtk::manage(new class Gtk::Label("It looks like this is your first time\n"
		"running Darimasen.\n\n"
		"It would be a good time to set up\n"
		"some default \"Run Actions\"\n\n"
		"Simply replace the wildcard (*)\n"
		"with your prefered program."));
   hbox1 = Gtk::manage(new class Gtk::HBox(false, 0));
   label2 = Gtk::manage(new class Gtk::Label("Text Files:"));
   label3 = Gtk::manage(new class Gtk::Label("Music FIles:"));
   label4 = Gtk::manage(new class Gtk::Label("Video Files:\n"
		""));
   label5 = Gtk::manage(new class Gtk::Label("Image files:\n"
		""));
   entry1 = Gtk::manage(new class Gtk::Entry());
   entry2 = Gtk::manage(new class Gtk::Entry());
   entry3 = Gtk::manage(new class Gtk::Entry());
   entry4 = Gtk::manage(new class Gtk::Entry());
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

   entry1->set_flags(Gtk::CAN_FOCUS);
   entry1->set_text("* \"$@\"");

   entry2->set_flags(Gtk::CAN_FOCUS);
   entry2->set_text("* \"$@\"");

   entry3->set_flags(Gtk::CAN_FOCUS);
   entry3->set_text("* \"$@\"");


   entry4->set_flags(Gtk::CAN_FOCUS);
   entry4->set_text("* \"$@\"");

   table1->set_border_width(2);
   table1->set_row_spacings(5);
   table1->set_col_spacings(4);
   table1->attach(*label2, 0, 1, 0, 1, Gtk::FILL);
   table1->attach(*label3, 0, 1, 1, 2, Gtk::FILL);
   table1->attach(*label4, 0, 1, 2, 3, Gtk::FILL);
   table1->attach(*label5, 0, 1, 3, 4, Gtk::FILL);
   table1->attach(*entry1, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL);
   table1->attach(*entry2, 1, 2, 1, 2, Gtk::EXPAND|Gtk::FILL);
   table1->attach(*entry3, 1, 2, 2, 3, Gtk::EXPAND|Gtk::FILL);
   table1->attach(*entry4, 1, 2, 3, 4, Gtk::EXPAND|Gtk::FILL);
   hbox2->pack_start(*table1);
   vbox1->pack_start(*hbox1);
   vbox1->pack_start(*hbox2);

   get_vbox()->pack_start(*vbox1);
   set_title("Darimasen: first run");


   add_action_widget(*okbutton1, -5);


   okbutton1->signal_clicked().connect(sigc::mem_fun(*this, &dialog1_glade::confirm));


   show_all_children();
}


/**********************/

void dialog1_glade::confirm(){

Glib::ustring choicesdir="/.choices";


  if ( entry1->get_text() != "* \"$@\""){ //don't intentionally make a broken script.
    Glib::ustring command = "#! /bin/sh\nexec " + entry1->get_text() + "\n";
    try {
      Glib::ustring exec1 = (getenv("HOME") +  (Glib::ustring)(choicesdir + "/MIME-types/text"));
      Gnome::Vfs::Handle write_handle;
      write_handle.create(exec1, Gnome::Vfs::OPEN_WRITE, false, 0755);
      write_handle.seek(Gnome::Vfs::SEEK_POS_START, 0);
      GnomeVFSFileSize bytes_written = write_handle.write(command.data(), command.size());
      }
    catch(const Gnome::Vfs::exception) {}
}

  if ( entry2->get_text() != "* \"$@\""){ //don't intentionally make a broken script.
    Glib::ustring command = "#! /bin/sh\nexec " + entry2->get_text() + "\n";
    try {
      Glib::ustring exec1 = (getenv("HOME") +  (Glib::ustring)(choicesdir + "/MIME-types/audio"));
      Gnome::Vfs::Handle write_handle;
      write_handle.create(exec1, Gnome::Vfs::OPEN_WRITE, false, 0755);
      write_handle.seek(Gnome::Vfs::SEEK_POS_START, 0);
      GnomeVFSFileSize bytes_written = write_handle.write(command.data(), command.size());
      }
    catch(const Gnome::Vfs::exception) {}
}

  if ( entry3->get_text() != "* \"$@\""){ //don't intentionally make a broken script.
    Glib::ustring command = "#! /bin/sh\nexec " + entry3->get_text() + "\n";
    try {
      Glib::ustring exec1 = (getenv("HOME") +  (Glib::ustring)(choicesdir + "/MIME-types/video"));
      Gnome::Vfs::Handle write_handle;
      write_handle.create(exec1, Gnome::Vfs::OPEN_WRITE, false, 0755);
      write_handle.seek(Gnome::Vfs::SEEK_POS_START, 0);
      GnomeVFSFileSize bytes_written = write_handle.write(command.data(), command.size());
      }
    catch(const Gnome::Vfs::exception) {}
}

  if ( entry4->get_text() != "* \"$@\""){ //don't intentionally make a broken script.
    Glib::ustring command = "#! /bin/sh\nexec " + entry4->get_text() + "\n";
    try {
      Glib::ustring exec1 = (getenv("HOME") +  (Glib::ustring)(choicesdir + "/MIME-types/image"));
      Gnome::Vfs::Handle write_handle;
      write_handle.create(exec1, Gnome::Vfs::OPEN_WRITE, false, 0755);
      write_handle.seek(Gnome::Vfs::SEEK_POS_START, 0);
      GnomeVFSFileSize bytes_written = write_handle.write(command.data(), command.size());
      }
    catch(const Gnome::Vfs::exception) {}
}

      hide();
      return;
      }
    





/**********************/
