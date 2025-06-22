#ifndef COMMAND_HELPERS_HPP
#define COMMAND_HELPERS_HPP
#include "dictionary.hpp"

namespace belyaev
{
  Dictionary* searchDictByName(Dictionaries& data, const std::string name);
  const Dictionary* searchDictByName(const Dictionaries& data, const std::string name); // ¯\_(ツ)_/¯
  bool isRuWordInDictionary(Dictionary& dictionary, const std::string word);
}

#endif