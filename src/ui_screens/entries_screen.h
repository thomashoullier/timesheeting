/** @file
 * @brief EntriesScreen definition. */
#ifndef ENTRIES_SCREEN_H
#define ENTRIES_SCREEN_H

#include "ui_component.h"
#include "entries_table.h"
#include "status_bar/status_bar_ncurses.h"
#include "stopwatch/stopwatch_ui.h"
#include "../logger_interface.h"
#include <memory>


/** @brief Entry UI screen. */
template <typename T_DB, typename T_LOG,
          typename =
              std::enable_if_t<std::is_base_of<DB_Interface, T_DB>::value &&
                               std::is_base_of<LoggerInterface, T_LOG>::value>>
class EntriesScreen : public UIComponent {
public:
  /** @brief Constructor. */
  explicit EntriesScreen(std::shared_ptr<T_DB> _db,
                         std::shared_ptr<StatusBarNCurses> _status)
    : stopwatch_ui(std::make_unique<StopwatchUI<T_DB>>(_db, _status)),
      entries_table(std::make_unique<EntriesTable<T_DB>>(_db, _status)),
      logger(&T_LOG::get()) {};

  char input_loop() override {
    UIComponent *cur_focus {entries_table.get()};
    while(true) {
      auto ch = cur_focus->input_loop();
      switch(ch) {
      case '\t':
        cur_focus = (cur_focus == stopwatch_ui.get()) ?
          entries_table.get() : stopwatch_ui.get();
      break;
      case '\n': // Update request is passed
        entries_table->update();
        break;
      default:
        return ch;
      }
    }
  };

  void refresh() override {
    stopwatch_ui->refresh();
    entries_table->refresh();
  };

  void clear() override {
    stopwatch_ui->clear();
    entries_table->clear();
  };

  void update() override {
    stopwatch_ui->update();
    entries_table->update();
  };

private:
  /** @brief Handle to the stopwatch. */
  std::unique_ptr<UIComponent> stopwatch_ui;
  /** @brief Handle to the table holding the entries. */
  std::unique_ptr<UIComponent> entries_table;
  /** @brief Logger interface. */
  LoggerInterface *logger;
};

#endif // ENTRIES_SCREEN_H
