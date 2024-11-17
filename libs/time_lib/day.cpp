#include "day.h"
#include "date.h"
#include <chrono>
#include "time_zone.h"

namespace time_lib {
  Day::Day() {
    Date utc_now{};
    auto now = TimeZone::get().zone->to_local(utc_now.tp);
    auto ld = std::chrono::floor<std::chrono::days>(now);
    ymd = std::chrono::year_month_day{ld};
  }

  Day::Day(std::chrono::year_month_day _ymd) : ymd{_ymd} {}

  void Day::next() { ymd = std::chrono::sys_days{ymd} + std::chrono::days{1}; }

  void Day::previous() {
    ymd = std::chrono::sys_days{ymd} - std::chrono::days{1};
  }

  DateRange Day::to_date_range() const {
    std::chrono::zoned_time day_begin{TimeZone::get().zone,
                                      std::chrono::sys_days{ymd}};
    std::chrono::zoned_time day_end{TimeZone::get().zone,
                                    std::chrono::sys_days{ymd} +
                                    std::chrono::days{1}};
    auto local_begin =
      std::chrono::floor<std::chrono::days>(day_begin.get_local_time());
    auto local_end =
      std::chrono::floor<std::chrono::days>(day_end.get_local_time());
    auto utc_begin = TimeZone::get().zone->to_sys(local_begin);
    auto utc_end = TimeZone::get().zone->to_sys(local_end);
    return DateRange{Date{utc_begin}, Date{utc_end}};
  }

  std::string Day::to_string() const {
    return std::format("{:%d%b%Y %a}", ymd);
  }
}
