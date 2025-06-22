#ifndef COMMAND_HANDLERS_HPP
#define COMMAND_HANDLERS_HPP
#include <functional>
#include <iostream>
#include "command-helpers.hpp"
#include "dictionary.hpp"

namespace belyaev
{
  void insertEntry(Dictionaries& data, std::istream& in, std::ostream& out);
  void printDict(const Dictionaries& data, std::istream& in, std::ostream& out);

  using commandMap = std::map<std::string, std::function<void(std::istream&, std::ostream&)>>;
  commandMap mapCommandHandlers(Dictionaries& data);
}

#endif
