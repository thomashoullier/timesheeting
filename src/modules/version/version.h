/** @file
 *  @brief Defining version constants for the program. */
#ifndef VERSION_H
#define VERSION_H

#include <string>

namespace version {
  /** @brief Version of the program. */
  static constexpr std::string TIMESHEETING_VERSION {"00.03dev"};
  /** @brief Version of the DB. */
  static constexpr int TIMESHEETING_DB_VERSION {1};
}

#endif // VERSION_H
