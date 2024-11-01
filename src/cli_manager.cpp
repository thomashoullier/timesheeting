#include "cli_manager.h"

CLIManager::CLIManager(const std::string &version_string)
    : cmd{TCLAP::CmdLine("", ' ', version_string)} {}

void CLIManager::add_flag(const std::string &flag, const std::string &long_flag,
                          const std::string &description, bool is_required) {
  TCLAP::ValueArg<std::string> arg(flag, long_flag, description,
                                   is_required, "", "string");
  cmd.add(arg);
}

void CLIManager::parse(int argc, const char *const *argv) {
  cmd.parse(argc, argv);
}
