#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <iostream>
#include <vector>
#include <string>
#include <map>

namespace belyaev
{
  struct Dictionary
  {
    std::map<std::string, std::string> dict;
  };
  std::string accumulatePairString(std::string result, const std::pair<const std::string, const std::string>& pair);
  std::ostream& operator<<(std::ostream& out, const Dictionary& dictionary);

  struct Dictionaries
  {
    std::map<std::string, Dictionary> dicts;
  };
}

#endif
