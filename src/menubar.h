#ifndef MENUBAR_H
#define MENUBAR_H

#include "about.h"
#include "gtkmm/popovermenubar.h"

class MenuBar : public Gtk::PopoverMenuBar {
public:
  MenuBar();

private:
  About about_dialog;

  void about_on_button_clicked();
};

#endif // MENUBAR_H
