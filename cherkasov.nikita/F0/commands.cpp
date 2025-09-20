#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

namespace cherkasov
{
  void makeDict(std::istream & in, DictTable & dicts)
  {
    std::string name;
    in >> name;
    if (dicts.find(name) != dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    dicts[name] = Dict{};
  }

  void listDicts(std::ostream & out, const DictTable & dicts)
  {
    if (dicts.empty())
    {
      out << "<EMPTY>" << '\n';
      return;
    }
    for (const auto & kv : dicts)
    {
      out << kv.first << '\n';
    }
  }

  void importText(std::istream & in, DictTable & dicts)
  {
    std::string name, filename;
    in >> name >> filename;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    std::ifstream file(filename);
    if (!file.is_open())
    {
      throw std::logic_error("<INVALID FILE>");
    }
    std::string word;
    while (file >> word)
    {
      ++(it->second[word]);
    }
  }

  void mergeDicts(std::istream & in, DictTable & dicts)
  {
    std::string res, d1, d2;
    in >> res >> d1 >> d2;
    if (dicts.find(res) != dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    if (dicts.find(d1) == dicts.end() || dicts.find(d2) == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    Dict merged = dicts[d1];
    for (auto & kv : dicts[d2])
    {
      merged[kv.first] += kv.second;
    }
    dicts[res] = std::move(merged);
  }

  void intersectDicts(std::istream & in, DictTable & dicts)
  {
    std::string res, d1, d2;
    in >> res >> d1 >> d2;
    if (dicts.find(res) != dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    if (dicts.find(d1) == dicts.end() || dicts.find(d2) == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    Dict result;
    for (auto & kv : dicts[d1])
    {
      auto it = dicts[d2].find(kv.first);
      if (it != dicts[d2].end())
      {
        result[kv.first] = std::min(kv.second, it->second);
      }
    }
    dicts[res] = std::move(result);
  }

  void cloneDict(std::istream & in, DictTable & dicts)
  {
    std::string res, src;
    in >> res >> src;
    if (dicts.find(res) != dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    auto it = dicts.find(src);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    dicts[res] = it->second;
  }

  void insertWord(std::istream & in, DictTable & dicts)
  {
    std::string name, word;
    int freq;
    in >> name >> word >> freq;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    if (freq <= 0)
    {
      throw std::logic_error("<INVALID NUMBER>");
    }
    if (it->second.find(word) != it->second.end())
    {
      throw std::logic_error("<INVALID WORD>");
    }
    it->second[word] = freq;
  }

  void dictSize(std::istream & in, std::ostream & out, const DictTable & dicts)
  {
    std::string name;
    in >> name;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    out << it->second.size() << '\n';
  }

  void eraseWord(std::istream & in, DictTable & dicts)
  {
    std::string name, word;
    in >> name >> word;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    if (it->second.erase(word) == 0)
    {
      throw std::logic_error("<INVALID WORD>");
    }
  }

  void clearDict(std::istream & in, DictTable & dicts)
  {
    std::string name;
    in >> name;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    it->second.clear();
  }

  void printTop(std::istream & in, std::ostream & out, const DictTable & dicts)
  {
    std::string name;
    int count;
    in >> name >> count;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    if (count <= 0 || count > static_cast<int>(it->second.size()))
    {
      throw std::logic_error("<INVALID NUMBER>");
    }
    std::vector< WordEntry > words(it->second.begin(), it->second.end());
    std::sort(words.begin(), words.end(),
      [](const WordEntry & a, const WordEntry & b)
      {
        return (a.second == b.second) ? (a.first < b.first) : (a.second > b.second);
      });
    for (int i = 0; i < count; ++i)
    {
      out << words[i].first << " " << words[i].second << '\n';
    }
  }

  void printRare(std::istream & in, std::ostream & out, const DictTable & dicts)
  {
    std::string name;
    int count;
    in >> name >> count;
    auto it = dicts.find(name);
    if (it == dicts.end())
    {
      throw std::logic_error("<INVALID DICTIONARY>");
    }
    if (count <= 0 || count > static_cast<int>(it->second.size()))
    {
      throw std::logic_error("<INVALID NUMBER>");
    }
    std::vector< WordEntry > words(it->second.begin(), it->second.end());
    std::sort(words.begin(), words.end(),
      [](const WordEntry & a, const WordEntry & b)
      {
        return (a.second == b.second) ? (a.first < b.first) : (a.second < b.second);
      });
    for (int i = 0; i < count; ++i)
    {
      out << words[i].first << " " << words[i].second << '\n';
    }
  }
}
