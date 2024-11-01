#include "cli.h"
#include "version.h"

namespace cli {
  CLIData parse(int argc, const char *const *argv) {
    std::string version_string =
      version::TIMESHEETING_VERSION + " built for DB v" +
      std::to_string(version::TIMESHEETING_DB_VERSION);
    TCLAP::CmdLine cmd("", ' ', version_string);
    TCLAP::ValueArg<std::string> config_file_arg
      ("c", "config_file", "Configuration file to use", false,
       "", "file_path");
    cmd.add(config_file_arg);
    cmd.parse(argc, argv);
    std::string config_file = config_file_arg.getValue();
    return CLIData{config_file};
  }
};
