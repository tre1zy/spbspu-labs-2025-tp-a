#ifndef FREQ_DICT_HPP
#define FREQ_DICT_HPP

#include <iosfwd>
#include <map>
#include <string>

namespace kostyukov
{
  struct FrequencyDictionary
  {
    std::map< std::string, size_t > counts;
    size_t totalWords = 0;
  };
  using FreqDictManager = std::map< std::string, FrequencyDictionary >;

  struct WordFreqPair
  {
    std::string word;
    double freq;
  };

  struct MapToPairTransformer
  {
    MapToPairTransformer(size_t totalWords);
    WordFreqPair operator()(const std::pair< const std::string, size_t >& mapPair) const;
    private:
      size_t totalWords_;
  };

  struct FreqComparator
  {
    explicit FreqComparator(bool ascending = false);
    bool operator()(const WordFreqPair& first, const WordFreqPair& second) const;
    private:
      bool ascending_;
  };

  struct PairPrinter
  {
    explicit PairPrinter(std::ostream& out);
    void operator()(const WordFreqPair&) const;
    private:
      std::ostream& out_;
  };

  struct KeyExtractor
  {
    std::string operator()(const std::pair< const std::string, size_t >& mapPair) const;
  };

  struct RemoveBatchPredicate
  {
    RemoveBatchPredicate(size_t total, bool less, double n);
    bool operator()(const std::pair< const std::string, size_t >& pair) const;
    private:
      size_t totalWords_;
      bool isLess_;
      double threshold_;
  };

  struct DictDeleter
  {
    explicit DictDeleter(FrequencyDictionary& dict);
    void operator()(const std::pair< const std::string, size_t >& pairToRemove) const;
    private:
      FrequencyDictionary& dict_;
  };

  struct SameWordPrinter
  {
    std::ostream& out;
    const FrequencyDictionary& dict1;
    const FrequencyDictionary& dict2;
    const std::string& name1;
    const std::string& name2;
    void operator()(const std::string& key) const;
  };

  struct IsInvalidChar
  {
    bool operator()(char c) const;
  };
  struct IsPunct
  {
    bool operator()(const char c) const;
  };
  struct WordProcessor
  {
    explicit WordProcessor(FrequencyDictionary& dict);
    void operator()(std::string word);
    private:
      FrequencyDictionary& dict_;
  };

  void createDict(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void top(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void bottom(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void deleteDict(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void loadDict(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void getFreq(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void removeBatch(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void findUniq(std::istream& in, std::ostream& out, FreqDictManager& dicts);
  void findSame(std::istream& in, std::ostream& out, FreqDictManager& dicts);
}
#endif
