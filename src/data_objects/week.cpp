#include "week.h"
#include "../time_zone.h"

Week::Week() {
  Date utc_now{};
  auto now = TimeZone::get().zone->to_local(utc_now.tp);
  auto ld = std::chrono::floor<std::chrono::days>(now);
  auto cur_day = std::chrono::weekday {ld};
  auto gap = cur_day - std::chrono::Monday;
  auto last_monday = ld - gap;
  ymd = std::chrono::year_month_day{last_monday};
}

void Week::next() { ymd = std::chrono::sys_days{ymd} + std::chrono::weeks{1}; }

void Week::previous() {
  ymd = std::chrono::sys_days{ymd} - std::chrono::weeks{1};
}

DateRange Week::to_date_range() const {
  std::chrono::zoned_time week_begin {TimeZone::get().zone,
                                      std::chrono::sys_days{ymd}};
  std::chrono::zoned_time week_end
    {TimeZone::get().zone,
     std::chrono::sys_days{ymd} + std::chrono::weeks{1}};
  auto local_begin =
      std::chrono::floor<std::chrono::days>(week_begin.get_local_time());
  auto local_end =
      std::chrono::floor<std::chrono::days>(week_end.get_local_time());
  auto utc_begin = TimeZone::get().zone->to_sys(local_begin);
  auto utc_end = TimeZone::get().zone->to_sys(local_end);
  return DateRange{Date{utc_begin}, Date{utc_end}};
}

std::string Week::to_string() const {
  return std::format("{:%d%b%Y W%V}", ymd);
}

std::array<Day, 7> Week::days() const {
  std::array<Day, 7> week_days;
  Day cur_day {ymd};
  for (auto i = 0 ; i < 7 ; ++i) {
    week_days.at(i) = cur_day;
    cur_day.next();
  }
  return week_days;
}
