#include <iostream>
#include "version.h"
#include <gtkmm.h>

class MyWindow : public Gtk::Window
{
public:
  MyWindow();
};

MyWindow::MyWindow()
{
  set_title("Basic application");
}

int main (int argc, char* argv[]) {
  std::cout << "# timesheeting main #" << std::endl;
  std::cout << "Version: " << TIMESHEETING_VERSION << std::endl;

  auto app = Gtk::Application::create("org.gtkmm.examples.base");
  return app->make_window_and_run<MyWindow>(argc, argv);
}
