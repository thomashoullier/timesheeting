#ifndef CLI_H
#define CLI_H

#include <tclap/CmdLine.h>

namespace cli {
  // Parse the CLI arguments.
  void parse(int argc, const char *const *argv);
};

#endif // CLI_H
