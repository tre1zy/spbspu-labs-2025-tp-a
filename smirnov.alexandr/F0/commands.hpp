#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "dictionary.hpp"

namespace smirnov
{
  void createCommand(Dicts & dicts, std::istream & in, std::ostream & out);
  void addCommand(Dicts & dicts, std::istream & in, std::ostream & out);
  void translateCommand(Dicts & dicts, std::istream & in, std::ostream & out);
  void removeCommand(Dicts & dicts, std::istream & in, std::ostream & out);
  void printCommand(Dicts & dicts, std::istream & in, std::ostream & out);
  void saveCommand(Dicts & dicts, std::istream & in, std::ostream & out);
  void mergeCommand(Dicts & dicts, std::istream & in, std::ostream & out);
  void deleteCommand(Dicts & dicts, std::istream & in, std::ostream & out);
  void editCommand(Dicts & dicts, std::istream & in, std::ostream & out);
  void renameCommand(Dicts & dicts, std::istream & in, std::ostream & out);
}
#endif
