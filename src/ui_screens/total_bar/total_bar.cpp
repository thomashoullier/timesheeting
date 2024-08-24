#include "total_bar.h"

TotalBar::TotalBar (const Duration &_duration)
  : BarNCurses(WindowPosition::top_right, WindowFormat::half_line),
    duration(_duration),
    display_string(_duration.to_string()) {}

void TotalBar::update(const Duration &_duration) {
  duration = _duration;
  display_string = _duration.to_string();
}

void TotalBar::refresh() const {
  BarNCurses::print(display_string);
}
