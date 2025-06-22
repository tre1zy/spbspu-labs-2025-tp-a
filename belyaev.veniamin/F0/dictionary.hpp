#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <iostream>
#include <vector>
#include <map>

namespace belyaev
{
  struct Dictionary
  {
    std::map<std::string, std::string> dictionary;
  };

  struct Dictionaries
  {
    std::map<std::string, Dictionary> dictionaries;
  };
}

#endif
