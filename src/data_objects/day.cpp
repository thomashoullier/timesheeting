#include "day.h"
#include "date.h"
#include <chrono>
#include <stdexcept>

Day::Day() {
  Date utc_now{};
  auto now = std::chrono::current_zone()->to_local(utc_now.tp);
  auto ld = std::chrono::floor<std::chrono::days>(now);
  ymd = std::chrono::year_month_day{ld};
}

void Day::next() {
  ymd = std::chrono::sys_days{ymd} + std::chrono::days{1};
}

void Day::previous() {
  ymd = std::chrono::sys_days{ymd} - std::chrono::days{1};
}

DateRange Day::to_date_range() const {
  std::chrono::zoned_time day_begin{std::chrono::current_zone(),
                                    std::chrono::sys_days{ymd}};
  std::chrono::zoned_time day_end{std::chrono::current_zone(),
                                  std::chrono::sys_days{ymd}
                                  + std::chrono::days{1}};
  auto local_begin = std::chrono::floor<std::chrono::days>
    (day_begin.get_local_time());
  auto local_end = std::chrono::floor<std::chrono::days>
    (day_end.get_local_time());
  auto utc_begin = std::chrono::current_zone()->to_sys(local_begin);
  auto utc_end = std::chrono::current_zone()->to_sys(local_end);
  return DateRange{Date{utc_begin}, Date{utc_end}};
}

std::string Day::to_string() const {
  return std::format("{:%d%b%Y}", ymd);
}
