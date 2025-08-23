#ifndef SEQUENCE_COMMANDS_HPP
#define SEQUENCE_COMMANDS_HPP

#include <ios>
#include <unordered_map>
#include "music.hpp"

namespace aleksandrov
{
  using Sequences = std::unordered_map< std::string, Sequence >;

  void printHelp(std::ostream&);
  void processCommands(std::istream&, std::ostream&, Sequences&);

  void listSeqs(std::ostream&, const Sequences&);
  void newSeq(std::istream&, Sequences&);
  void loadSeq(std::istream&, Sequences&);
  void printSeq(std::istream&, std::ostream&, const Sequences&);
}

#endif

