/** @file
 * @brief Status bar interface implementation using ncurses. */
#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "ncurses_lib/bar_ncurses.h"
#include <vector>
#include <string>

namespace tui {
  /** @brief Status bar implementation using ncurses.

   Singleton pattern. */
  class StatusBarNCurses final : public ncurses_lib::BarNCurses {
  public:
    /** @brief Grab the status bar singleton. */
    static StatusBarNCurses& get();

    /** @brief Print the message in the status bar. */
    void print(const std::string &msg) final;
    /** @brief Print the message in the status bar, and then pause until user
     * input. */
    void print_wait(const std::string &msg);
    /** @brief Ask for confirmation. */
    bool query_confirmation (const std::string &msg);
    /** @brief Get a new string from the user.

     Returns an empty string on cancellation. */
    std::string get_user_string();
    /** @brief Get a user string, with suggestions. */
    std::string get_user_string_suggestions
    (const std::vector<std::string> &suggestions);

    /** @brief Resize the status bar. Leave the edit mode if we were in it. */
    void resize() final;

    StatusBarNCurses(StatusBarNCurses const&) = delete;
    void operator=(StatusBarNCurses const&) = delete;

  private:
    /** @brief Controls the switch between normal and edit mode. */
    bool user_wants_to_input = false;
    /** @brief Constructor. */
    explicit StatusBarNCurses();
    /** @brief Sanitize the strings inputted by the users. */
    std::string sanitize_input(const std::string &input) const;
  };

  /** @brief Grab the status bar. */
  StatusBarNCurses& status();
} // namespace tui

#endif // STATUS_BAR_H
