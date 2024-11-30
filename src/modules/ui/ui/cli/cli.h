/** @file
 *  @brief CLIData definition. */
#ifndef CLI_H
#define CLI_H

#include <tclap/CmdLine.h>
#include <filesystem>

namespace cli {
  /** @brief Parsed CLI data. */
  struct CLIData {
    /** @brief Path to the config file. */
    std::filesystem::path config_filepath;
    /** @brief Beginning date for the DB export. */
    std::string beg_date;
    /** @brief End date for the DB export. */
    std::string end_date;
    /** @brief Path for the file to export the DB to. */
    std::filesystem::path export_file;
  };

  /** @brief Parse the CLI arguments. */
  CLIData parse(int argc, const char *const *argv);
};

#endif // CLI_H
