#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <map>
#include <string>
#include <utility>

namespace shak
{
  struct Word
  {
    std::string word;
    size_t frequency;
  };

  struct FrequencyDictionary
  {
    std::map< std::string, size_t > dictionary;
    size_t size = 0;
  };

  std::string printPair(const std::pair< std::string, size_t > &p);
  bool uniqueOnce(const std::pair< const std::string, size_t > &p);
  bool pairEqual(const std::pair< const std::string, size_t > &p1, const std::pair< const std::string, size_t > &p2);
}
#endif
