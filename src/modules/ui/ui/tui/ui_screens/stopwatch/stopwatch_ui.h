/** @file
 * @brief StopwatchUI definition. */
#ifndef STOPWATCH_UI_H
#define STOPWATCH_UI_H

#include "../ui_component.h"
#include "stopwatch_ncurses.h"
#include "../date_selector/day_selector.h"
#include <memory>

namespace tui {
  /** @brief High-level UI for the stopwatch. */
  class StopwatchUI final : public UIComponent {
  public:
    /** @brief Constructor. */
    explicit StopwatchUI (std::shared_ptr<DaySelector> _day_selector);

    config::NormalActions input_loop() override;
    void refresh() final;
    void clear() override;
    void resize() final;
    void update() override;
    void update_status() override;

  private:
    /** @brief Handle to the low-level stopwatch element. */
    StopwatchNcurses stopwatch;
    /** @brief Pointer to the day selector of the register.
        Used to get the currently selected day. */
    std::shared_ptr<DaySelector> day_selector;

    /** @brief Rename a field in the entry in staging. */
    void rename_item();
    /** @brief Set the current field, if it is a date, to now().
        Return true if anything has changed, false otherwise. */
    bool set_current_now ();
  };
} // namespace tui

#endif // STOPWATCH_UI_H
