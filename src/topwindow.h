#ifndef TOPWINDOW_H
#define TOPWINDOW_H

#include <gtkmm.h>
#include "globals.h"
#include "menubar.h"
#include "tabs.h"

class TopWindow : public Gtk::Window {
public:
  TopWindow();

protected:
  // Child widgets:
  Gtk::Box m_VBox;

  MenuBar menubar;
  Tabs tabs;
};

#endif // TOPWINDOW_H
