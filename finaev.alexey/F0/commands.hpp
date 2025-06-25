#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <map>
#include <unordered_map>
#include <string>
#include <functional>
#include <algorithm>
#include "openningInfo.hpp"

namespace finaev
{
  using globalDebuts = std::unordered_map< std::string, finaev::OpenningInfo >;
  using debutsBases = std::unordered_map< std::string, std::unordered_map< std::string, bool > >;
  using mapOfCommands = std::map< std::string, std::function< void() > >;

  void create_debut(std::istream&, std::ostream&, globalDebuts&);
  void create_base(std::istream&, std::ostream&, debutsBases&);
  void add(std::istream&, std::ostream&, const globalDebuts&, debutsBases&); 
  void exact_find(std::istream&, std::ostream&, const globalDebuts&, const debutsBases&);

  mapOfCommands createCommandsHandler(std::istream& in, std::ostream& out, globalDebuts&, debutsBases&);
}

#endif
