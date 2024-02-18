#include <iostream>

#include "application.h"
#include "globals.h"

int main() {
  std::cout << "# timesheeting main #" << std::endl;
  std::cout << "Version: " << TIMESHEETING_VERSION << std::endl;

  Application app;
  return app.run_gui();
}
