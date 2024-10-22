#include "total_bar.h"
#include "../../duration_displayer.h"

TotalBar::TotalBar (const Duration &_duration)
  : BarNCurses(WindowPosition::top_right, WindowFormat::half_line),
    duration(_duration),
    display_string(DurationDisplayer::get().to_string(_duration)) {}

void TotalBar::update(const Duration &_duration) {
  duration = _duration;
  display_string = DurationDisplayer::get().to_string(_duration);
}

void TotalBar::refresh() const {
  BarNCurses::print_right(display_string);
}
