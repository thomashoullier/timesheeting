/** @file
 *  @brief UIScreen definition. */
#ifndef UI_SCREEN_H
#define UI_SCREEN_H

#include "ui_component.h"

namespace tui {
  /** @brief Parent type of all the main UI screens. */
  class UIScreen : public UIComponent {
  protected:
    /** @brief Does the screen currently need an update? */
    bool needs_update = false;

  public:
    virtual ~UIScreen () = default;

    /** @brief Signal to the current screen that it needs an update since state
        has changed elsewhere. */
    void signal_update_need();
  };
} // namespace tui

#endif // UI_SCREEN_H
