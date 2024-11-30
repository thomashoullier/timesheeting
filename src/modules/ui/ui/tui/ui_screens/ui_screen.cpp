#include "ui_screen.h"

namespace tui {
  void UIScreen::signal_update_need() {
    needs_update = true;
  }
} // namespace tui
