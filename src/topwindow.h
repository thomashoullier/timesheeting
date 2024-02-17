#ifndef TOPWINDOW_H
#define TOPWINDOW_H

#include <gtkmm.h>
#include "globals.h"
#include "about.h"

class TopWindow : public Gtk::Window {
public:
  TopWindow();

protected:
  // Signal handlers:
  void on_button_clicked();

  // Child widgets:
  Gtk::Box m_VBox;
  Gtk::Box m_ButtonBox;
  Gtk::Button m_Button;
  About about_dialog;
};

#endif // TOPWINDOW_H
