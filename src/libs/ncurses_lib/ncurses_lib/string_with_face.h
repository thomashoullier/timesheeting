/** @file
 *  @brief StringWithFace definition. */
#ifndef STRING_WITH_FACE_H
#define STRING_WITH_FACE_H

#include <string>

namespace ncurses_lib {
  /** @brief String object with a face setting. */
  class StringWithFace {
  public:
    /** @brief String content. */
    std::string str;
    /** @brief Should this string be displayed with a highlight? */
    bool highlight;

    /** @brief Constructor. */
    explicit StringWithFace(const std::string &_str, bool _highlight = false)
      : str(_str), highlight(_highlight) {};
  };
}
#endif // STRING_WITH_FACE_H
