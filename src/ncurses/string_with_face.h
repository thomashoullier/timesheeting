#ifndef STRING_WITH_FACE_H
#define STRING_WITH_FACE_H

#include <string>

/** @brief String object with a face setting. */
class StringWithFace {
public:
  std::string str;
  bool highlight;

  StringWithFace(const std::string &_str, bool _highlight = false)
    : str(_str), highlight(_highlight) {};
};

#endif // STRING_WITH_FACE_H
