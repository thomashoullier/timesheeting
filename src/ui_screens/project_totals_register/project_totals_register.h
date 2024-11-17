#ifndef PROJECT_TOTALS_REGISTER_H
#define PROJECT_TOTALS_REGISTER_H

#include "core/project_total.h"
#include "ncurses_lib/menu_ncurses.h"
#include "../ui_component.h"

/** @brief ProjectTotal register implementation in ncurses. */
class ProjectTotalsRegister : public ncurses_lib::MenuNCurses,
                              public UIComponent {
public:
  /** @brief Constructor. */
  ProjectTotalsRegister(const std::vector<core::ProjectTotal> &totals);
  /** @brief Replace the totals currently displayed. */
  void set_items(const std::vector<core::ProjectTotal> &totals);
  char input_loop () override;
  void refresh() override;
  void clear() override;
  void update() override;

private:
  /** @brief Get display strings from the provided set of items. */
  std::vector<std::string> items_to_string
  (const std::vector<core::ProjectTotal> &items);
};

#endif // PROJECT_TOTALS_REGISTER_H