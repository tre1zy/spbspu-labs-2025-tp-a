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

  struct WordPairGeneratorIterator
  {
  public:
    using value_type = std::pair< std::string, std::list< std::string > >;
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

    WordPairGeneratorIterator(const WordPairGeneratorIterator& other) = default;

    WordPairGeneratorIterator& operator=(const WordPairGeneratorIterator& other) = default;

    WordPairGeneratorIterator& operator++()
    {
      value = value_type();
      std::string englishWord;
      if (*file >> englishWord)
      {
        std::list< std::string > russianWords;
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

    reference operator*() const
    {
      return value;
    }

    pointer operator->() const
    {
      return &value;
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

  struct DictMerger
  {
    using Pair = std::pair< std::string, std::list< std::string > >;
    orlova::Dictionary newDict;
    DictMerger(const orlova::Dictionary& dict):
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

  struct CompareKeys
  {
    bool operator()(const std::pair< std::string, orlova::Translations >& pair1, const std::pair< std::string, orlova::Translations >& pair2)
    {
      return pair1.first < pair2.first;
    }
  };

  struct KeyExtractor
  {
    using Pair = std::pair< std::string, std::list < std::string > >;
    std::string operator()(const Pair& p) const
    {
      return p.first;
    }
  };

  struct PairTransformer
  {
    using Pair = std::pair< std::string, std::list< std::string > >;
    const orlova::Dictionary& dict1;
    const orlova::Dictionary& dict2;

    PairTransformer(const orlova::Dictionary& d1,
      const orlova::Dictionary& d2):
      dict1(d1),
      dict2(d2)
    {}

    Pair operator()(std::string& key)
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

  struct ResidualTransform
  {
    const orlova::Dictionary& dict1;
    ResidualTransform(const orlova::Dictionary& d1):
      dict1(d1)
    {}
    using Pair = std::pair< std::string, std::list< std::string > >;
    Pair operator()(std::string& key) const
    {
      return std::make_pair(key, dict1.at(key));
    }
  };
}

template<>
struct std::iterator_traits< WordPairGeneratorIterator >
{
  using value_type = WordPairGeneratorIterator::value_type;
  using pointer = WordPairGeneratorIterator::pointer;
  using reference = WordPairGeneratorIterator::reference;
  using iterator_category = WordPairGeneratorIterator::iterator_category;
};

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
  std::string dictName, englishWord;
  std::list< std::string > russianWords;
  in >> dictName >> englishWord;
  std::istream_iterator< std::string > temp(in), eof;
  std::copy(temp, eof, std::back_inserter(russianWords));
  auto it = dicts.find(dictName);
  if (!dictionaryExists(dicts, dictName) || it->second.find(englishWord) != it->second.end())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  it->second[englishWord] = russianWords;
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
  Dictionary newDict;
  WordPairGeneratorIterator generator(file);
  std::copy(generator, WordPairGeneratorIterator{}, std::inserter(newDict, newDict.begin()));
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
  auto& dict1 = dicts.at(dictName1);
  auto& dict2 = dicts.at(dictName2);
  if (dict1.empty() && dict2.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  Dictionary newDict;
  std::copy(dict1.begin(), dict1.end(), std::inserter(newDict, newDict.begin()));
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
  auto& dict = dicts.at(dictName);
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
  auto& dict = dicts.at(dictName);
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
  auto& dict = dicts.at(dictName);
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
  std::set_intersection(dict1.begin(), dict1.end(), dict2.begin(), dict2.end(), std::inserter(temp, temp.end()), CompareKeys{});
  std::transform(temp.begin(), temp.end(), std::inserter(newDict, newDict.end()), std::bind(intersectListDict, _1, dict2));
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
  const auto& dict1 = dicts.at(dictName1);
  const auto& dict2 = dicts.at(dictName2);
  std::list< std::string > keys1, keys2, unique_keys;
  std::transform(dict1.begin(), dict1.end(), std::inserter(keys1, keys1.begin()), KeyExtractor{});
  std::transform(dict2.begin(), dict2.end(), std::inserter(keys2, keys2.begin()), KeyExtractor{});
  std::set_symmetric_difference(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), std::inserter(unique_keys, unique_keys.begin()));
  if (unique_keys.empty())
  {
    out << "<DICTIONARIES ARE SIMILAR>\n";
    return;
  }
  Dictionary newDict;
  PairTransformer transformer(dict1, dict2);
  std::transform(unique_keys.begin(), unique_keys.end(), std::inserter(newDict, newDict.begin()), transformer);
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
  const auto& dict1 = dicts.at(dictName1);
  const auto& dict2 = dicts.at(dictName2);
  std::list< std::string > keys1, keys2, difference;
  std::transform(dict1.begin(), dict1.end(), std::inserter(keys1, keys1.begin()), KeyExtractor{});
  std::transform(dict2.begin(), dict2.end(), std::inserter(keys2, keys2.begin()), KeyExtractor{});
  std::set_difference(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), std::inserter(difference, difference.begin()));
  if (difference.empty())
  {
    out << "<DICTIONARIES ARE SIMILAR>\n";
    return;
  }
  Dictionary newDict;
  ResidualTransform transformer(dict1);
  std::transform(difference.begin(), difference.end(), std::inserter(newDict, newDict.begin()), transformer);
  dicts[newDictName] = newDict;
  out << "<RESIDUAL DICTIONARY CREATED>\n";
}
