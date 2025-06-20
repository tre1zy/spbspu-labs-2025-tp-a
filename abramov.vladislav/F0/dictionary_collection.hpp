#ifndef DICTIONARY_COLLECTION_HPP
#define DICTIONARY_COLLECTION_HPP
#include "dictionary.hpp"

namespace abramov
{
  struct DictionaryCollection
  {
    DictionaryCollection();
    void addDict(const std::string &name);
    void deleteDict(const std::string &name);
  private:
    std::unordered_map< std::string, Dictionary > collection_;
  };
}

#endif
