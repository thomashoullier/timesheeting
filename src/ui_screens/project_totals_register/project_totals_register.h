#ifndef PROJECT_TOTALS_REGISTER_H
#define PROJECT_TOTALS_REGISTER_H

#include "../../data_objects/project_total.h"
#include "../../ncurses/menu_ncurses.h"
#include "../ui_component.h"
#include "../status_bar/status_bar_ncurses.h"

/** @brief ProjectTotal register implementation in ncurses. */
class ProjectTotalsRegister : public MenuNCurses,
                              public UIComponent {
public:
  /** @brief Constructor. */
  ProjectTotalsRegister(const std::vector<ProjectTotal> &totals);
  /** @brief Replace the totals currently displayed. */
  void set_items(const std::vector<ProjectTotal> &totals);
  char input_loop () override;
  void refresh() override;
  void clear() override;
  void update() override;

private:
  /** @brief Get display strings from the provided set of items. */
  std::vector<std::string> items_to_string
  (const std::vector<ProjectTotal> &items);
};

#endif // PROJECT_TOTALS_REGISTER_H
