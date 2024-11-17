#ifndef DURATION_DISPLAYER_H
#define DURATION_DISPLAYER_H

#include "duration.h"

namespace time_lib {
  /** @brief Singleton for formatting duration according to varied formats. */
  class DurationDisplayer {
  public:
    static DurationDisplayer& get(float hours_per_day = 0.0);

    /** @brief Cycle the current display format. */
    void cycle_format();
    /** @brief Get a string representation of a Duration in the current
        display format. */
    std::string to_string(const time_lib::Duration &dur);
    /** @brief Get a short string representation of a Duration in the current
        display format. */
    std::string to_shortstring(const time_lib::Duration &dur);

  private:
    enum class DurationFormat { hours, days, seconds };

    DurationFormat current_format;
    float hours_per_day;

    DurationDisplayer(float hours_per_day);

  };
}
#endif //DURATION_DISPLAYER_H
