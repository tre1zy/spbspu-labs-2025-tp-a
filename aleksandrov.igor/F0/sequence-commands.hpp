#ifndef SEQUENCE_COMMANDS_HPP
#define SEQUENCE_COMMANDS_HPP

#include <ios>

namespace aleksandrov
{
  void printHelp(std::ostream&);
  void processCommands(std::istream&, std::ostream&);
}

#endif

