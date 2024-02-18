#ifndef TOPWINDOW_H
#define TOPWINDOW_H

#include "globals.h"
#include "menubar.h"
#include "tabs.h"

#include "gtkmm/box.h"
#include "gtkmm/window.h"
#include "gtkmm/enums.h"

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
