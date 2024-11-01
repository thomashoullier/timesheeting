#include "cli.h"
#include "version.h"

namespace cli {
  void parse(int argc, const char *const *argv) {
    CLIManager cli_man (version::TIMESHEETING_VERSION
                        + " built for DB v" +
                        std::to_string(version::TIMESHEETING_DB_VERSION));
    cli_man.parse(argc, argv);
  }
};
