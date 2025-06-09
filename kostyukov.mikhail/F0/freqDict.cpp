#include "freqDict.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <iterator>
#include <string>

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
      dict_.total_words++;
    }
  }

  void createDict(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dict_name;
    in >> dict_name;
    if (!in || dict_name.empty())
    {
      out << "<MISSING ARGUMENTS>";
      return;
    }
    if(!isValidName(dict_name))
    {
      out << "INVALID DICTIONARY NAME>";
      return;
    }
    if (dicts.count(dict_name))
    {
      out << "<DICTIONARY EXISTS>";
      return;
    }
    dicts.emplace(dict_name, FrequencyDictionary{});
    out << "successfully created " << dict_name;
  }

  void loadDict(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dict_name;
    std::string filename;
    in >> dict_name >> filename;
    if (!in || dict_name.empty() || filename.empty())
    {
      out << "<MISSING ARGUMENTS>";
      return;
    }
    if (!isValidName(dict_name))
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
    FrequencyDictionary& dict = dicts[dict_name];
    dict.counts.clear();
    dict.total_words = 0;
    std::istream_iterator< std::string > start(file);
    std::istream_iterator< std::string > end;
    std::for_each(start, end, WordProcessor(dict));
    out << "successfully loaded " << dict_name;
  }

  void getFreq(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dict_name;
    std::string word;
    in >> dict_name >> word;
    if (!in || dict_name.empty() || word.empty())
    {
      out << "<MISSING ARGUMENTS>";
      return;
    }
    if (!isValidName(dict_name))
    {
      out << "<INVALID DICTIONARY NAME>";
      return;
    }
    auto dict_iter = dicts.find(dict_name);
    if (dict_iter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>";
      return;
    }
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    word.erase(std::remove_if(word.begin(), word.end(), IsPunct{}), word.end());
    const FrequencyDictionary& dict = dict_iter->second;
    auto word_iter = dict.counts.find(word);
    if (word_iter == dict.counts.end() || dict.total_words == 0)
    {
      out << "0.0%";
      return;
    }
    double freq = (static_cast< double >(word_iter->second) / dict.total_words) * 100.0;
    ScopeGuard scopeGrd(out);
    out << std::fixed << std::setprecision(1) << freq << "%";
  }

  void deleteDict(std::istream& in, std::ostream& out, FreqDictManager& dicts)
  {
    std::string dict_name;
    in >> dict_name;
    if (!in || dict_name.empty())
    {
      out << "<MISSING ARGUMENTS>";
      return;
    }
    if (!isValidName(dict_name))
    {
      out << "<INVALID DICTIONARY NAME>";
      return;
    }
    auto iter = dicts.find(dict_name);
    if (iter == dicts.end())
    {
      out << "<DICTIONARY NOT FOUND>";
      return;
    }
    dicts.erase(iter);
    out << "successfully deleted " << dict_name;
  }
}
