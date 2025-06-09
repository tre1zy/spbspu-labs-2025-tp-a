#ifndef FREQ_DICT_HPP
#define FREQ_DICT_HPP

#include <iosfwd>
#include <map>

namespace kostyukov
{
  struct FrequencyDictionary
  {
    std::map< std::string, size_t > counts;
    size_t total_words = 0;
  };
  using FreqDictManager = std::map< std::string, FrequencyDictionary >;

  struct IsInvalidChar
  {
    bool operator()(char c) const;
  };

  void createDict(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void deleteDict(std::istream& in, std::ostream& out, FreqDictManager& dicts);
}

#endif
