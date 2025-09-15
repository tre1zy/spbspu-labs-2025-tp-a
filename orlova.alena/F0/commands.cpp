#include "commands.h"
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>
#include <functional>
#include <cstddef>

namespace
{
  bool dictionaryExists(const orlova::Dictionaries& dicts, const std::string& name)
  {
    return dicts.find(name) != dicts.end();
  }

  void printTranslations(std::ostream& out, const orlova::Translations& translations)
  {
    if (translations.empty())
    {
      return;
    }
    auto it = translations.begin();
    out << *it;
    std::advance(it, 1);
    std::copy(it, translations.end(), std::ostream_iterator< std::string >(out, ", "));
  }

  orlova::Translations intersectLists(orlova::Translations& list1, orlova::Translations& list2)
  {
    list1.sort();
    list2.sort();
    orlova::Translations res;
    std::set_intersection(list1.begin(), list1.end(), list2.begin(), list2.end(), std::back_inserter(res));
    return res;
  }
  std::pair< std::string, orlova::Translations > intersectListDict(const std::pair< std::string, orlova::Translations >& pair,
    const orlova::Dictionary& intersected)
  {
    orlova::Translations list1 = pair.second;
    orlova::Translations list2 = intersected.at(pair.first);
    orlova::Translations res = intersectLists(list1, list2);
    return { pair.first, res };
  }
}

void orlova::addEmptyDictionary(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string name;
  in >> name;
  if (!in || dicts.find(name) != dicts.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  dicts[name] = Dictionary();
  out << "<DICTIONARY ADDED>\n";
}

void orlova::addTranslation(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName, englishWord, russianWord;
  in >> dictName >> englishWord >> russianWord;
  auto it = dicts.find(dictName);
  if (it == dicts.end() || it->second.find(englishWord) != it->second.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Translations translation;
  translation.push_back(russianWord);
  it->second[englishWord] = translation;
  out << "<TRANSLATION ADDED>\n";
}

void orlova::addDictionary(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName;
  std::string fileName;
  in >> dictName >> fileName;
  if (dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  class WordPairGeneratorIterator
  {
  public:
    using value_type = std::pair< std::string, std::list< std::string > >;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::input_iterator_tag;

    WordPairGeneratorIterator():
      file(nullptr),
      end(true)
    {}

    WordPairGeneratorIterator(std::istream& f):
      file(&f),
      end(false)
    {
      ++(*this);
    }

    WordPairGeneratorIterator& operator++()
    {
      value = value_type();
      std::string englishWord;
      if (*file >> englishWord)
      {
        std::list< std::string > russianWords;
        std::string word;
        std::istream_iterator< std::string > it(*file), eof;
        std::copy(it, eof, std::back_inserter(russianWords));
        value = std::make_pair(englishWord, russianWords);
      }
      else
      {
        end = true;
      }
      return *this;
    }

    const value_type& operator*() const
    {
      return value;
    }

    explicit operator bool() const
    {
      return !end;
    }

    bool operator==(const WordPairGeneratorIterator& other) const
    {
      return end == other.end;
    }

    bool operator!=(const WordPairGeneratorIterator& other) const
    {
      return !(*this == other);
    }

  private:
    std::istream* file = nullptr;
    value_type value;
    bool end = true;
  };

  Dictionary newDict;
  WordPairGeneratorIterator generator(file);
  std::copy(generator, WordPairGeneratorIterator{}, std::back_inserter(newDict));
  dicts[dictName] = newDict;
  file.close();
  out << "<DICTIONARY ADDED FROM FILE>\n";
}

void orlova::find(std::istream& in, std::ostream& out, const Dictionaries& dicts)
{
  std::string dictName;
  std::string englishWord;
  in >> dictName >> englishWord;
  if (!dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const auto& dict = dicts.at(dictName);
  auto it = dict.find(englishWord);
  if (it == dict.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  printTranslations(out, it->second);
}

void orlova::merge(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName1;
  std::string dictName2;
  std::string newDictName;
  in >> dictName1 >> dictName2 >> newDictName;
  if (!dictionaryExists(dicts, dictName1) || !dictionaryExists(dicts, dictName2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it1 = dicts.find(dictName1);
  auto& dict1 = it1->second;
  auto it2 = dicts.find(dictName2);
  auto& dict2 = it2->second;
  if (dict1.empty() && dict2.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  Dictionary newDict;
  std::copy(dict1.begin(), dict1.end(), std::back_inserter(newDict));

  struct DictMerger
  {
    using Pair = std::pair< std::string, std::list < std::string > >;
    Dictionary newDict;
    DictMerger(const Dictionary& dict):
      newDict(dict)
    {}
    Pair operator()(const Pair& pair)
    {
      auto it = newDict.find(pair.first);
      if (it != newDict.end())
      {
        it->second.insert(it->second.end(), pair.second.begin(), pair.second.end());
        return *it;
      }
      else
      {
        newDict[pair.first] = pair.second;
        return pair;
      }
    }
  };

  DictMerger merger(newDict);
  std::transform(dict2.begin(), dict2.end(), std::inserter(newDict, newDict.end()), merger);
  dicts[newDictName] = newDict;
  out << "<DICTIONARIES MERGED>\n";
}

void orlova::clear(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName;
  in >> dictName;
  if (!dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it = dicts.find(dictName);
  auto& dict = it->second;
  if (dict.empty())
  {
    out << "<ALREADY EMPTY>\n";
    return;
  }
  dict.clear();
  out << "<DICTIONARY CLEARED>\n";
}

void orlova::removeWord(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName;
  std::string englishWord;
  in >> dictName >> englishWord;
  if (!dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  auto& dict = dictIt->second;
  auto it = dict.find(englishWord);
  if (it == dict.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  dict.erase(it);
  out << "<WORD REMOVED>\n";
}

void orlova::append(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName;
  std::string englishWord;
  std::string newTranslation;
  in >> dictName >> englishWord >> newTranslation;
  if (!dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  auto& dict = dictIt->second;
  auto it = dict.find(englishWord);
  if (it == dict.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  Translations translation;
  translation.push_back(newTranslation);
  it->second = translation;
  out << "<TRANSLATION UPDATED>\n";
}

void orlova::dictionarySize(std::istream& in, std::ostream& out, const Dictionaries& dicts)
{
  std::string dictName;
  in >> dictName;
  if (!dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const auto& dict = dicts.at(dictName);
  out << dict.size();
}

void orlova::intersectionOfDicts(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName1;
  std::string dictName2;
  std::string newDictName;
  in >> dictName1 >> dictName2 >> newDictName;
  if (!dictionaryExists(dicts, dictName1) || !dictionaryExists(dicts, dictName2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dict1 = dicts.at(dictName1);
  auto dict2 = dicts.at(dictName2);
  if (dict1.empty() || dict2.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  Dictionary temp, newDict;
  auto d_first = std::inserter(temp, temp.end());

  struct CompareKeys
  {
    bool operator()(const std::pair< std::string, Translations >& pair1, const std::pair< std::string, Translations >& pair2)
    {
      return pair1.first < pair2.first;
    }
  };
  std::set_intersection(dict1.begin(), dict1.end(), dict2.begin(), dict2.end(), d_first, CompareKeys{});
  d_first = std::inserter(newDict, newDict.end());
  std::transform(temp.begin(), temp.end(), d_first, std::bind(intersectListDict, _1, dict2));
  dicts[newDictName] = newDict;
  out << "<SUCCESSFULLY INTERSECTED>";
}

void orlova::deleteDictionary(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName;
  in >> dictName;

  if (!dictionaryExists(dicts, dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  dicts.erase(dictName);
  out << "<DICTIONARY DELETED>\n";
}

void orlova::nonrepeatingWords(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName1;
  std::string dictName2;
  std::string newDictName;
  in >> dictName1 >> dictName2 >> newDictName;
  if (!dictionaryExists(dicts, dictName1) || !dictionaryExists(dicts, dictName2) || dictionaryExists(dicts, newDictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it1 = dicts.find(dictName1);
  auto it2 = dicts.find(dictName2);
  const auto& dict1 = it1->second;
  const auto& dict2 = it2->second;

  struct DictTransformer
  {
    using Pair = std::pair< std::string, std::list < std::string > >;
    std::string operator()(const Pair& p) const
    {
      return p.first;
    }
  };

  std::set< std::string > keys1, keys2, unique_keys;
  std::transform(dict1.begin(), dict1.end(), std::inserter(keys1, keys1.end()), DictTransformer{});
  std::transform(dict2.begin(), dict2.end(), std::inserter(keys2, keys2.end()), DictTransformer{});
  std::set_symmetric_difference(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), std::back_inserter(unique_keys));
  if (unique_keys.empty())
  {
    out << "<DICTIONARIES ARE SIMILAR>\n";
    return;
  }
  Dictionary newDict;

  struct PairTransformer
  {
    using Pair = std::pair< std::string, std::list < std::string > >;
    const Dictionary& dict1;
    const Dictionary& dict2;

    PairTransformer(const Dictionary& d1,
      const Dictionary& d2):
      dict1(d1),
      dict2(d2)
    {}

    Pair operator()(const std::string& key)
    {
      if (dict1.find(key) != dict1.end())
      {
        return { key, dict1.at(key) };
      }
      else
      {
        return { key, dict2.at(key) };
      }
    }
  };

  PairTransformer transformer(dict1, dict2);
  std::transform(unique_keys.begin(), unique_keys.end(), std::back_inserter(newDict), transformer);
  dicts[newDictName] = newDict;
  out << "<NONREPEATING WORDS COLLECTED>\n";
}

void orlova::residual(std::istream& in, std::ostream& out, Dictionaries& dicts)
{
  std::string dictName1;
  std::string dictName2;
  std::string newDictName;
  in >> dictName1 >> dictName2 >> newDictName;
  if (!dictionaryExists(dicts, dictName1) || !dictionaryExists(dicts, dictName2) || dictionaryExists(dicts, newDictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto it1 = dicts.find(dictName1);
  auto it2 = dicts.find(dictName2);
  const auto& dict1 = it1->second;
  const auto& dict2 = it2->second;
  std::set< std::string > keys1, keys2, difference;
  std::copy(dict1.begin(), dict1.end(), std::back_inserter(keys1));
  std::copy(dict2.begin(), dict2.end(), std::back_inserter(keys2));
  std::set_difference(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), std::back_inserter(difference));
  if (difference.empty())
  {
    out << "<DICTIONARIES ARE SIMILAR>\n";
    return;
  }
  Dictionary newDict;

  struct ResidualTransform
  {
    const Dictionary& dict1;
    ResidualTransform(const Dictionary& d1):
      dict1(d1)
    {}
    using Pair = std::pair< std::string, std::list < std::string > >;
    Pair operator()(const std::string& key) const
    {
      return std::make_pair(key, dict1.at(key));
    }
  };

  ResidualTransform transformer(dict1);
  std::transform(difference.begin(), difference.end(), std::back_inserter(newDict), transformer);
  dicts[newDictName] = newDict;
  out << "<RESIDUAL DICTIONARY CREATED>\n";
}
