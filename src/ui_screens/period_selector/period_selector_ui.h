#ifndef PERIOD_SELECTOR_UI_H
#define PERIOD_SELECTOR_UI_H

#include "period_selector_ncurses.h"
#include "../ui_component.h"
#include "../status_bar/status_bar_ncurses.h"
#include <memory>

class PeriodSelectorUI : public UIComponent {
public:
  /** @brief Constructor. */
  explicit PeriodSelectorUI ();
  char input_loop() override;
  void refresh() override;
  void clear() override;
  void update() override;

  DateRange get_current_date_range();

private:
  /** @brief Handle to the low-level period selector element. */
  PeriodSelectorNCurses period_selector;

  /** @brief Rename a field. */
  void rename_item ();
};

#endif // PERIOD_SELECTOR_UI_H
