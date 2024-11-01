#ifndef CLI_MANAGER_H
#define CLI_MANAGER_H

#include "tclap/CmdLine.h"
#include <string>

/** @brief CLI Manager class. This decouples TCLAP from the rest of the
    program. */
class CLIManager {
public:
  CLIManager(const std::string &version_string);

  void add_flag(const std::string &flag,
                const std::string &long_flag,
                const std::string &description,
                bool is_required);
  void parse(int argc, const char *const *argv);

private:
  TCLAP::CmdLine cmd;
};

#endif // CLI_MANAGER_H
