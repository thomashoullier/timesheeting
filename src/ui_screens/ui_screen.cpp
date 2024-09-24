#include "ui_screen.h"

void UIScreen::signal_update_need() {
  needs_update = true;
}
