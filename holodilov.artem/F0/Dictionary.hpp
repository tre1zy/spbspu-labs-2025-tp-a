#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <list>
#include <string>
#include <map>
#include <iostream>

namespace holodilov
{
  using MapDictionary = std::map< std::string, std::list< std::string > >;

  struct Dictionary
  {
    std::string name;
    std::string lang;
    MapDictionary dict;

    void printTranslations(std::ostream& out, const std::string& englishWord);
  };

  std::ostream& operator<<(std::ostream& out, const Dictionary& dict);
  std::istream& operator>>(std::istream& in, Dictionary& dict);

  using MapDictionaries = std::map< std::string, Dictionary >;
  using MapDictionariesPair = std::pair< const std::string, Dictionary >;
}

#endif
