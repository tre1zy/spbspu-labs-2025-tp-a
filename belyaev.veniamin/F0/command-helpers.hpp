#ifndef COMMAND_HELPERS_HPP
#define COMMAND_HELPERS_HPP
#include "dictionary.hpp"

namespace belyaev
{
  Dictionary* searchDictByName(Dictionaries& data, const std::string& name);
  const Dictionary* searchDictByName(const Dictionaries& data, const std::string& name); // ¯\_(ツ)_/¯
  bool insertEntry(Dictionary& currentDictionary, std::string russianWord, std::string translation);
  bool removeEntry(Dictionary& currentDictionary, std::string russianWord);

  using dictionaryIterator = std::_Rb_tree_const_iterator<entryPair>;
  bool isTranslationInEntry(const std::pair<const std::string, std::string> entry, const std::string& translation);
  dictionaryIterator getItOfWordInDictByRu(const Dictionary& dictionary, const std::string& ruWord);
  dictionaryIterator getItOfWordInDictByEn(const Dictionary& dictionary, const std::string& enWord);
  bool isRuWordInDictionary(const Dictionary& dictionary, dictionaryIterator itRuWord);
  bool isEnWordInDictionary(const Dictionary& dictionary, dictionaryIterator itEnWord);

  bool pairContainsRuChars(const entryPair entry, std::string subStringRu);
  bool pairContainsEnChars(const entryPair entry, std::string subStringEn);

  const Dictionary& printAllHelper(const std::pair<const std::string, Dictionary>& dictsEntry, std::ostream& out);
}

#endif