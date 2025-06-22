#include "command-helpers.hpp"

belyaev::Dictionary* belyaev::searchDictByName(Dictionaries& data, const std::string name)
{
  auto result = data.dicts.find(name);
  if (result == data.dicts.end())
  {
    return nullptr;
  }
  return &(result->second);
}

const belyaev::Dictionary* belyaev::searchDictByName(const Dictionaries& data, const std::string name)
{
  auto result = data.dicts.find(name);
  if (result == data.dicts.end())
  {
    return nullptr;
  }
  return &(result->second);
}

bool belyaev::isRuWordInDictionary(Dictionary& dictionary, const std::string word)
{
  auto result = dictionary.dict.find(word);
  if (result == dictionary.dict.end())
  {
    return false;
  }
  return true;
}