#ifndef COMMAND_HELPERS_HPP
#define COMMAND_HELPERS_HPP
#include "dictionary.hpp"

namespace belyaev
{
  Dictionary* searchDictByName(Dictionaries& data, const std::string& name);
  const Dictionary* searchDictByName(const Dictionaries& data, const std::string& name); // ¯\_(ツ)_/¯

  using dictionaryIterator = std::_Rb_tree_const_iterator<std::pair<const std::string, std::string>>;
  bool isTranslationInEntry(const std::pair<const std::string, std::string> entry, const std::string& translation);
  dictionaryIterator getItOfWordInDictByRu(const Dictionary& dictionary, const std::string& ruWord);
  dictionaryIterator getItOfWordInDictByEn(const Dictionary& dictionary, const std::string& enWord);
  bool isRuWordInDictionary(const Dictionary& dictionary, dictionaryIterator itRuWord);
  bool isEnWordInDictionary(const Dictionary& dictionary, dictionaryIterator itEnWord);
}

#endif