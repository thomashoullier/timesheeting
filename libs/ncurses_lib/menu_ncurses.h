/** @file
 * @brief MenuNCurses definition. */
#ifndef MENU_NCURSES_H
#define MENU_NCURSES_H

#include "win_ncurses.h"
#include "string_with_face.h"
#include <exception>
#include <menu.h>
#include <vector>
#include <string>

namespace ncurses_lib {
  /** @brief A ncurses window holding a generic menu. */
  class MenuNCurses : public WinNCurses {
  public:
    /** @brief Construct the menu holding the provided display strings,
        at the given position, with the given format and number of columns. */
    MenuNCurses(const std::vector<std::string> &items,
                WindowPosition winpos, WindowFormat winformat,
                int _ncols);
    /** @brief Constructor with explicitely provided short display strings. */
    MenuNCurses(const std::vector<std::string> &items,
                const std::vector<std::string> &short_items,
                WindowPosition winpos, WindowFormat winformat, int _ncols);
    /** @brief Constructor with explicitely provided short display strings
        with face. */
    MenuNCurses(const std::vector<std::string> &items,
                const std::vector<StringWithFace> &short_items,
                WindowPosition winpos, WindowFormat winformat, int _ncols);
    /** @brief Destructor. */
    ~MenuNCurses();

    /** @brief Select the item down of the current one. */
    void select_down_item();
    /** @brief Select the item up of the current one. */
    void select_up_item();
    /** @brief Select the item right of the current one. */
    void select_right_item();
    /** @brief Select the item left of the current one. */
    void select_left_item();
    /** @brief Refresh the display. */
    void refresh();
    /** @brief Clear the display. */
    void clear();
    /** @brief Replace the currently held items. */
    void set_items(const std::vector<std::string> &items);
    /** @brief Replace the currently held items and short items. */
    void set_items(const std::vector<std::string> &items,
                   const std::vector<std::string> &short_items);
    /** @brief Replace the currently held items and short items with a face. */
    void set_items(const std::vector<std::string> &items,
                   const std::vector<StringWithFace> &short_items);
    /** @brief Get the full display string for the current item. */
    const std::string& get_current_item_string() const;
    /** @brief Draw a single border on the menu window. */
    void set_border();
    /** @brief Erase the border on the menu window. */
    void unset_border();

  protected:
    /** @brief Get the current item index. */
    int get_item_index() const;
    /** @brief Get the current row index. */
    int get_row_index() const;
    /** @brief Get the current column index. */
    int get_col_index() const;

  private:
    /** @brief Number of columms in the menu. */
    int ncols;
    /** @brief Size of the display cell for each item. */
    const int item_width;
    /** @brief ncurses menu object. */
    MENU *menu;
    /** @brief ncurses menu items. */
    std::vector<ITEM *> menu_items;
    /** @brief Persistent storage for the strings being displayed.

        This is needed by ncurses, otherwise the display points
        to whatever memory. */
    std::vector<std::string> display_strings;
    /** @brief Shortened display strings for display in-menu only. */
    std::vector<std::string> short_display_strings;
    /** @brief String displayed for showing empty items. */
    std::string empty_string;

    /** @brief Constructor helper: initialize the menu. */
    void init_menu();
    /** @brief Initialize the menu items. */
    void init_items(const std::vector<std::string> &items);
    /** @brief Initialize the menu items and explicit short items. */
    void init_items(const std::vector<std::string> &items,
                    const std::vector<std::string> &short_items);
    /** @brief Initialize the menu items and explicit short items with face. */
    void init_items(const std::vector<std::string> &items,
                    const std::vector<StringWithFace> &short_items);
    /** @brief Initialize the menu UI. */
    void init_menu_window();
    /** @brief Destructor helper. Also called on update. */
    void destroy_menu ();
    /** @brief Crop or pad a string to match a fixed size. */
    std::string crop_pad_str (const std::string &str, int len);
  };

  /** @brief Exception when encountering an empty menu. */
  class MenuEmpty : public std::exception {
  private:
    /** @brief Exception message. */
    std::string msg;

  public:
    /** @brief Exception constructor. */
    MenuEmpty (const char* _msg) : msg(_msg) {};
    /** @brief Mandatory std::exception method. */
    const char* what() const throw() { return msg.c_str(); };
  };
}
#endif // MENU_NCURSES_H
