#ifndef MENUBAR_H
#define MENUBAR_H

#include "about.h"
#include <gtkmm.h>

class MenuBar {
public:
  MenuBar();
  Gtk::PopoverMenuBar *widget;
private:
  About about_dialog;

  void about_on_button_clicked();
};

#endif // MENUBAR_H
