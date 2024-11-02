#include "cli.h"
#include "version.h"

namespace cli {
  CLIData parse(int argc, const char *const *argv) {
    std::string version_string =
      version::TIMESHEETING_VERSION + " built for DB v" +
      std::to_string(version::TIMESHEETING_DB_VERSION);
    TCLAP::CmdLine cmd("", ' ', version_string);
    TCLAP::ValueArg<std::string> config_file_arg("c", "config_file",
                                                 "Configuration file to use",
                                                 false, "", "file_path");
    TCLAP::ValueArg<std::string> beg_date_arg("b", "begin_date",
                                              "Export begin date",
                                              false, "", "26Oct2024 08:00:00");
    TCLAP::ValueArg<std::string> end_date_arg("e", "end_date",
                                              "Export end date",
                                              false, "", "27Oct2024 08:00:00");
    TCLAP::ValueArg<std::string> export_file_arg("o", "output_file",
                                                 "Export file",
                                                 false, "", "file_path");
    cmd.add(export_file_arg);
    cmd.add(end_date_arg);
    cmd.add(beg_date_arg);
    cmd.add(config_file_arg);
    cmd.parse(argc, argv);
    return CLIData{config_file_arg.getValue(),
                   beg_date_arg.getValue(),
                   end_date_arg.getValue(),
                   export_file_arg.getValue()};
  }
};
