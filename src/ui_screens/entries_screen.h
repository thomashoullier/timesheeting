/** @file
 * @brief EntriesScreen definition. */
#ifndef ENTRIES_SCREEN_H
#define ENTRIES_SCREEN_H

#include "ui_component.h"
#include <memory>

/** @brief Entry UI screen. */
class EntriesScreen : public UIComponent {
public:
  /** @brief Constructor. */
  explicit EntriesScreen();

  char input_loop() override;
  void refresh() override;
  void clear() override;
  void update() override;

private:
  /** @brief Handle to the stopwatch. */
  std::unique_ptr<UIComponent> stopwatch_ui;
  /** @brief Handle to the table holding the entries. */
  std::unique_ptr<UIComponent> entries_table;
};

#endif // ENTRIES_SCREEN_H
