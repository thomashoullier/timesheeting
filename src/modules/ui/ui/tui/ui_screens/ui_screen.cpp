#include "ui_screen.h"

namespace tui {
  void UIScreen::signal_update_need() {
    needs_update = true;
  }

  void UIScreen::signal_resize_need() {
    needs_resize = true;
  }
} // namespace tui
