#include "entries_screen.h"
#include "entries_table.h"
#include "stopwatch/stopwatch_ui.h"
#include "log_lib/logger.h"
#include "ui/keys/bound_keys.h"

namespace tui {
  EntriesScreen::EntriesScreen()
    : stopwatch_ui(std::make_unique<StopwatchUI>()),
      entries_table(std::make_unique<EntriesTable>()) {}

  char EntriesScreen::input_loop() {
    UIComponent *cur_focus{entries_table.get()};
    while (true) {
      auto ch = cur_focus->input_loop();
      auto kb = keys::BoundKeys::get().kb;
      if (kb.subtabs.bound_to(ch)) {
        cur_focus = (cur_focus == stopwatch_ui.get()) ? entries_table.get()
          : stopwatch_ui.get();
      } else if (kb.validate.bound_to(ch)) { // Update request is passed
        entries_table->update();
      } else {
        return ch;
      }
    }
  }

  void EntriesScreen::refresh() {
    if (needs_update)
      update();
    stopwatch_ui->refresh();
    entries_table->refresh();
  };

  void EntriesScreen::clear() {
    stopwatch_ui->clear();
    entries_table->clear();
  };

  void EntriesScreen::update() {
    log_lib::logger().log("EntriesScreen update.",
                          log_lib::LogLevel::debug);
    stopwatch_ui->update();
    entries_table->update();
    needs_update = false;
  };
} // namespace tui
