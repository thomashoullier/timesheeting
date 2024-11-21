/** @file
 * @brief StopwatchUI definition. */
#ifndef STOPWATCH_UI_H
#define STOPWATCH_UI_H

#include "../ui_component.h"
#include "stopwatch_ncurses.h"

namespace tui {
  /** @brief High-level UI for the stopwatch. */
  class StopwatchUI : public UIComponent {
  public:
    /** @brief Constructor. */
    explicit StopwatchUI ();

    char input_loop() override;
    void refresh() override;
    void clear() override;
    void update() override;

  private:
    /** @brief Handle to the low-level stopwatch element. */
    StopwatchNcurses stopwatch;

    /** @brief Rename a field in the entry in staging. */
    void rename_item();
    /** @brief Set the current field, if it is a date, to now(). */
    void set_current_now ();
  };
} // namespace tui

#endif // STOPWATCH_UI_H
