#ifndef CLI_H
#define CLI_H

#include "cli_manager.h"

namespace cli {
  // Parse the CLI arguments.
  void parse(int argc, const char *const *argv);
};

#endif // CLI_H
