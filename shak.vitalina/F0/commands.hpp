#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "dictionary.hpp"

namespace shak
{
  using Dictionaries = std::map< std::string, FrequencyDictionary >;
  void createDict(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void clearDict(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void deleteDict(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void printDict(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void insertW(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void removeW(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void getFreq(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void getUniqe(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void equal(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void compare(Dictionaries &dicts, std::istream &in, std::ostream &out);
}
#endif
