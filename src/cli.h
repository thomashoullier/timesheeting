#ifndef CLI_H
#define CLI_H

#include <tclap/CmdLine.h>
#include <filesystem>

namespace cli {
  /** @brief Parsed CLI data. */
  struct CLIData {
    std::filesystem::path config_filepath;
  };

  /** @brief Parse the CLI arguments. */
  CLIData parse(int argc, const char *const *argv);
};

#endif // CLI_H
