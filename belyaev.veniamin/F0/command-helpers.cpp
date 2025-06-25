#include "command-helpers.hpp"
#include <functional>
#include <algorithm>

belyaev::Dictionary* belyaev::searchDictByName(Dictionaries& data, const std::string& name)
{
  auto result = data.dicts.find(name);
  if (result == data.dicts.end())
  {
    return nullptr;
  }
  return &(result->second);
}

const belyaev::Dictionary* belyaev::searchDictByName(const Dictionaries& data, const std::string& name)
{
  auto result = data.dicts.find(name);
  if (result == data.dicts.end())
  {
    return nullptr;
  }
  return &(result->second);
}

bool belyaev::isTranslationInEntry(const std::pair<const std::string, std::string> entry, const std::string& translation)
{
  return entry.second == translation;
}

belyaev::dictionaryIterator belyaev::getItOfWordInDictByRu(const Dictionary& dictionary, const std::string& ruWord)
{
  return dictionary.dict.find(ruWord);
}

belyaev::dictionaryIterator belyaev::getItOfWordInDictByEn(const Dictionary& dictionary, const std::string& enWord)
{
  using namespace std::placeholders;
  auto isTranslationInEntryBind = std::bind(isTranslationInEntry, _1, enWord);
  return std::find_if(dictionary.dict.begin(), dictionary.dict.end(), isTranslationInEntryBind);
}

bool belyaev::isRuWordInDictionary(const Dictionary& dictionary, dictionaryIterator itRuWord)
{
  return itRuWord != dictionary.dict.end();
}

bool belyaev::isEnWordInDictionary(const Dictionary& dictionary, dictionaryIterator itEnWord)
{
  return itEnWord != dictionary.dict.end();
}

bool belyaev::pairContainsRuChars(const entryPair entry, std::string subStringRu)
{
  return entry.first.find(subStringRu) != std::string::npos;
}

bool belyaev::pairContainsEnChars(const entryPair entry, std::string subStringEn)
{
  return entry.second.find(subStringEn) != std::string::npos;
}

const belyaev::Dictionary& belyaev::printAllHelper(const std::pair<const std::string, Dictionary>& dictsEntry, std::ostream& out)
{
  out << "Dictionary " << dictsEntry.first << ":\n";
  return dictsEntry.second;
}