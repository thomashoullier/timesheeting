#include "total_bar.h"
#include "time_lib/duration_displayer.h"

TotalBar::TotalBar (const time_lib::Duration &_duration)
  : BarNCurses(WindowPosition::top_right, WindowFormat::half_line),
    duration(_duration),
    display_string(time_lib::DurationDisplayer::get().to_string(_duration)) {}

void TotalBar::update(const time_lib::Duration &_duration) {
  duration = _duration;
  display_string = time_lib::DurationDisplayer::get().to_string(_duration);
}

void TotalBar::refresh() const {
  BarNCurses::print_right(display_string);
}
