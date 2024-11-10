/** @file
 * @brief Status bar interface implementation using ncurses. */
#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "../../ncurses/bar_ncurses.h"

/** @brief Status bar implementation using ncurses. */
class StatusBarNCurses : public BarNCurses {
public:
  /** @brief Grab the status bar singleton. */
  static StatusBarNCurses& get();

  /** @brief Print the message in the status bar. */
  void print(const std::string &msg);
  /** @brief Print the message in the status bar, and then pause until user
   * input. */
  void print_wait(const std::string &msg);
  /** @brief Ask for confirmation. */
  bool query_confirmation (const std::string &msg);
  /** @brief Get a new string from the user. */
  std::string get_user_string();

private:
  /** @brief Constructor. */
  StatusBarNCurses();
  /** @brief Sanitize the strings inputted by the users. */
  std::string sanitize_input(const std::string &input) const;
};

/** @brief Grab the status bar. */
StatusBarNCurses& status();

#endif // STATUS_BAR_H
