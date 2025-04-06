/** @file
 *  @brief DurationDisplayer definition. */
#ifndef DURATION_DISPLAYER_H
#define DURATION_DISPLAYER_H

#include <string>
#include "duration.h"

namespace time_lib {
  /** @brief Singleton for formatting duration according to varied formats. */
  class DurationDisplayer {
  public:
    /** @brief Grab the singleton. Initialize on the first call. */
    static DurationDisplayer& get(float hours_per_day = 0.0);

    /** @brief Cycle the current display format. */
    void cycle_format();
    /** @brief Get a string representation of a Duration in the current
        display format. */
    std::string to_string(const Duration &dur);
    /** @brief Get a short string representation of a Duration in the current
        display format. */
    std::string to_shortstring(const Duration &dur);

  private:
    /** @brief The accepted formats for duration display. */
    enum class DurationFormat { hours, days, seconds };

    /** @brief The format currently displayed. */
    DurationFormat current_format;
    /** @brief Parameter for displaying days: how many hours are there
               in a working day?*/
    float hours_per_day;

    /** @brief Singleton constructor. */
    explicit DurationDisplayer(float hours_per_day);
  };
}
#endif //DURATION_DISPLAYER_H
