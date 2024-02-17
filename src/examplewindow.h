#ifndef EXAMPLEWINDOW_H
#define EXAMPLEWINDOW_H

#include <gtkmm.h>
#include "globals.h"
#include "about.h"

class ExampleWindow : public Gtk::Window {
public:
  ExampleWindow();
  virtual ~ExampleWindow();

protected:
  // Signal handlers:
  void on_button_clicked();

  // Child widgets:
  Gtk::Box m_VBox;
  Gtk::Box m_ButtonBox;
  Gtk::Button m_Button;
  About m_Dialog;
};

#endif // EXAMPLEWINDOW_H
