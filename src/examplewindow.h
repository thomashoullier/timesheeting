#ifndef EXAMPLEWINDOW_H
#define EXAMPLEWINDOW_H

#include <gtkmm.h>

class ExampleWindow : public Gtk::Window {
public:
  ExampleWindow();
  virtual ~ExampleWindow();

protected:
  // Signal handlers:
  void on_button_clicked();

  // Child widgets:
  Gtk::Box m_VBox;
  Gtk::Label m_Label;
  Gtk::Box m_ButtonBox;
  Gtk::Button m_Button;
  Gtk::AboutDialog m_Dialog;
};

#endif // EXAMPLEWINDOW_H
