#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <map>
#include <unordered_map>
#include <string>
#include <functional>
#include "openningInfo.hpp"

namespace finaev
{
  using globalDebuts = std::unordered_map< std::string, finaev::OpenningInfo >;
  using debutsBase = std::unordered_map< std::string, std::unordered_map< std::string, bool > >;
  using mapOfCommands = std::map< std::string, std::function< void() > >;
  void create_debut(std::istream&, std::ostream&, globalDebuts&);
  mapOfCommands createCommandsHandler(std::istream& in, std::ostream& out, globalDebuts&/*, debutsBase&*/);
}

#endif