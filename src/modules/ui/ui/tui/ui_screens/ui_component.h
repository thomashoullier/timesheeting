/** @file
 * @brief UIComponent definition. */
#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "config/key.h"
namespace tui {
  /** @brief Interface class for a UI component. */
  class UIComponent {
  public:
    virtual ~UIComponent () = default;
    /** @brief Refresh the component's display. */
    virtual void refresh () = 0;
    /** @brief Clear the component from the display. */
    virtual void clear () = 0;
    /** @brief Resize the component according to current terminal dimensions. */
    virtual void resize() = 0;
    /** @brief UI input loop for the component. */
    virtual config::NormalActions input_loop () = 0;
    /** @brief Update the state of the component
        to reflect the latest program state. */
    virtual void update () = 0;
    /** @brief Update the status bar with respect to current selection. */
    virtual void update_status() = 0;
  };
} // namespace tui

#endif // UI_COMPONENT_H
