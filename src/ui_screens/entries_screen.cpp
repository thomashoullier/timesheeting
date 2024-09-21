#include "entries_screen.h"
#include "entries_table.h"
#include "stopwatch/stopwatch_ui.h"

EntriesScreen::EntriesScreen()
    : stopwatch_ui(std::make_unique<StopwatchUI>()),
      entries_table(std::make_unique<EntriesTable>()) {}

char EntriesScreen::input_loop() {
  UIComponent *cur_focus{entries_table.get()};
  while (true) {
    auto ch = cur_focus->input_loop();
    switch (ch) {
    case '\t':
      cur_focus = (cur_focus == stopwatch_ui.get()) ? entries_table.get()
                                                    : stopwatch_ui.get();
      break;
    case '\n': // Update request is passed
      entries_table->update();
      break;
    default:
      return ch;
    }
  }
}

void EntriesScreen::refresh() {
  stopwatch_ui->refresh();
  entries_table->refresh();
};

void EntriesScreen::clear() {
  stopwatch_ui->clear();
  entries_table->clear();
};

void EntriesScreen::update() {
  stopwatch_ui->update();
  entries_table->update();
};
