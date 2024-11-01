#include "cli.h"
#include "version.h"

namespace cli {
  void parse(int argc, const char *const *argv) {
    std::string version_string =
      version::TIMESHEETING_VERSION + " built for DB v" +
      std::to_string(version::TIMESHEETING_DB_VERSION);
    TCLAP::CmdLine cmd("", ' ', version_string);
    cmd.parse(argc, argv);
  }
};
