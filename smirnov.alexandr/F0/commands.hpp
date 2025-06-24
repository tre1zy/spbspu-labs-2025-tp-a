#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "dictionary.hpp"

namespace smirnov
{
  void createCommand(Dicts & dicts, std::istream & in, std::ostream & out);
  void addCommand(Dicts & dicts, std::istream & in, std::ostream & out);
  void translateCommand(Dicts & dicts, std::istream & in, std::ostream & out);
}
#endif
