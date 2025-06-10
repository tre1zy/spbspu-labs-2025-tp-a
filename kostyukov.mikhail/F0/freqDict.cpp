#include "freqDict.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>

#include "scopeGuard.hpp"

namespace kostyukov
{
  bool IsInvalidChar::operator()(char c) const
  {
    return !std::isalnum(c) && c != '_';
  }
  bool IsPunct::operator()(char c) const
  {
    return std::ispunct(c);
  }

  bool isValidName(const std::string& name)
  {
    if (name.empty() || !std::isalpha(name[0]))
    {
      return false;
    }
    return std::find_if(name.begin(), name.end(), IsInvalidChar{}) == name.end();
  }

  WordProcessor::WordProcessor(FrequencyDictionary& dict):
    dict_(dict)
  {}

  void WordProcessor::operator()(std::string word)
  {
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    word.erase(std::remove_if(word.begin(), word.end(), IsPunct{}), word.end());
    if (!word.empty())
    {
      dict_.counts[word]++;
      dict_.totalWords++;
    }
  }

  MapToPairTransformer::MapToPairTransformer(size_t totalWords):
    totalWords_(totalWords)
  {}

  WordFreqPair MapToPairTransformer::operator()(const std::pair< const std::string, size_t >& mapPair) const
  {
    double freq = (totalWords_ == 0) ? 0.0 : (static_cast< double >(mapPair.second) / totalWords_) * 100.0;
    return WordFreqPair{ mapPair.first, freq };
  }

  FreqComparator::FreqComparator(bool ascending):
    ascending_(ascending)
  {}

  bool FreqComparator::operator()(const WordFreqPair& first, const WordFreqPair& second) const
  {
    return (ascending_) ? first.freq < second.freq : first.freq > second.freq;
  }

  PairPrinter::PairPrinter(std::ostream& out):
    out_(out)
  {}

  void PairPrinter::operator()(const WordFreqPair& pair) const
  {
    ScopeGuard scopeGrd(out_);
    out_ << pair.word << " - " << std::fixed << std::setprecision(1) << pair.freq << "%\n";
  }

  void getTopOrBottom(std::istream& in, std::ostream& out, FreqDictManager& dicts, bool isBottom)
  {
    std::string dictName;
    size_t n = 0;
    in >> dictName >> n;
    if (!in || dictName.empty() || n == 0)
    {
      out << "<MISSING ARGUMENT>";
      return;
    }
    if (!isValidName(dictName))
    {
      out << "<INVALID DICTIONARY NAME>";
      return;
    }
    auto dictIter = dicts.find(dictName);
    if (dictIter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>";
      return;
    }
    const FrequencyDictionary& dict = dictIter->second;
    if (dict.counts.empty())
    {
      out << "<EMPTY DICTIONARY>";
      return;
    }
    std::vector< WordFreqPair > tempVec;
    tempVec.reserve(dict.counts.size());
    auto begin = dict.counts.begin();
    auto end = dict.counts.end();
    std::transform(begin, end, std::back_inserter(tempVec), MapToPairTransformer(dict.totalWords));
    size_t limit = std::min(n, tempVec.size());
    std::partial_sort(tempVec.begin(), tempVec.begin() + limit, tempVec.end(), FreqComparator(isBottom));
    std::for_each(tempVec.begin(), tempVec.begin() + limit, PairPrinter(out));
  }

  void top(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    getTopOrBottom(in, out, dicts, false);
  }

  void bottom(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    getTopOrBottom(in, out, dicts, true);
  }

  void createDict(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dictName;
    in >> dictName;
    if (!in || dictName.empty())
    {
      out << "<MISSING ARGUMENTS>";
      return;
    }
    if(!isValidName(dictName))
    {
      out << "INVALID DICTIONARY NAME>";
      return;
    }
    if (dicts.count(dictName))
    {
      out << "<DICTIONARY EXISTS>";
      return;
    }
    dicts.emplace(dictName, FrequencyDictionary{});
    out << "successfully created " << dictName;
  }

  void loadDict(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dictName;
    std::string filename;
    in >> dictName >> filename;
    if (!in || dictName.empty() || filename.empty())
    {
      out << "<MISSING ARGUMENTS>";
      return;
    }
    if (!isValidName(dictName))
    {
      out << "<INVALID DICTIONARY NAME>";
      return;
    }
    std::fstream file(filename);
    if (!file)
    {
      out << "<FILE ERROR>";
      return;
    }
    FrequencyDictionary& dict = dicts[dictName];
    dict.counts.clear();
    dict.totalWords = 0;
    std::istream_iterator< std::string > start(file);
    std::istream_iterator< std::string > end;
    std::for_each(start, end, WordProcessor(dict));
    out << "successfully loaded " << dictName;
  }

  void getFreq(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dictName;
    std::string word;
    in >> dictName >> word;
    if (!in || dictName.empty() || word.empty())
    {
      out << "<MISSING ARGUMENTS>";
      return;
    }
    if (!isValidName(dictName))
    {
      out << "<INVALID DICTIONARY NAME>";
      return;
    }
    auto dictIter = dicts.find(dictName);
    if (dictIter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>";
      return;
    }
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    word.erase(std::remove_if(word.begin(), word.end(), IsPunct{}), word.end());
    const FrequencyDictionary& dict = dictIter->second;
    auto wordIter = dict.counts.find(word);
    if (wordIter == dict.counts.end() || dict.totalWords == 0)
    {
      out << "0.0%";
      return;
    }
    double freq = (static_cast< double >(wordIter->second) / dict.totalWords) * 100.0;
    ScopeGuard scopeGrd(out);
    out << std::fixed << std::setprecision(1) << freq << "%";
  }

  void deleteDict(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dictName;
    in >> dictName;
    if (!in || dictName.empty())
    {
      out << "<MISSING ARGUMENTS>";
      return;
    }
    if (!isValidName(dictName))
    {
      out << "<INVALID DICTIONARY NAME>";
      return;
    }
    auto iter = dicts.find(dictName);
    if (iter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>";
      return;
    }
    dicts.erase(iter);
    out << "successfully deleted " << dictName;
  }

  RemoveBatchPredicate::RemoveBatchPredicate(size_t total, bool less, double n):
    totalWords_(total),
    isLess_(less),
    threshold_(n)
  {}

  bool RemoveBatchPredicate::operator()(const std::pair< const std::string, size_t >& pair) const
  {
    if (totalWords_ == 0)
    {
      return false;
    }
    double freq = (static_cast< double >(pair.second) / totalWords_) * 100.0;
    return isLess_ ? (freq < threshold_) : (freq > threshold_);
  }

  DictDeleter::DictDeleter(FrequencyDictionary& dict):
    dict_(dict)
  {}

  void DictDeleter::operator()(const std::pair< const std::string, size_t >& pairToRemove) const
  {
    dict_.totalWords -= pairToRemove.second;
    dict_.counts.erase(pairToRemove.first);
  }

  void removeBatch(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dictName;
    std::string param;
    double n = 0;
    in >> dictName >> param >> n;
    if (!in || dictName.empty() || param.empty() || n == 0)
    {
      out << "<MISSING ARGUMENT>";
      return;
    }
    if (!isValidName(dictName))
    {
      out << "<INVALID DICTIONARY NAME>";
      return;
    }
    auto dictIter = dicts.find(dictName);
    if (dictIter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>";
      return;
    }
    if (n < 0 || (param != "freq_less" && param != "freq_more"))
    {
      out << "<INVALID ARGUMENT>";
      return;
    }
    FrequencyDictionary& dict = dictIter->second;
    if (dict.counts.empty())
    {
      out << "<EMPTY DICTIONARY>";
      return;
    }
    std::vector< std::pair< const std::string, size_t > > pairsToRemove;
    auto begin = dict.counts.begin();
    auto end = dict.counts.end();
    auto pred = RemoveBatchPredicate(dict.totalWords, param == "freq_less", n);
    std::copy_if(begin, end, std::back_inserter(pairsToRemove),pred);
    if (pairsToRemove.empty())
    {
      out << "no words to remove";
      return;
    }
    std::for_each(pairsToRemove.begin(), pairsToRemove.end(), DictDeleter(dict));
    out << "Removed " << pairsToRemove.size() << "words.";
  }
}
