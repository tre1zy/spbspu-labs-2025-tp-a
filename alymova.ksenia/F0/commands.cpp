#include "commands.hpp"
#include <iostream>
#include <string>
#include <exception>
#include "dict-utils.hpp"

namespace
{
  std::list< std::string >::const_iterator findTranslate(const alymova::TranslateSet& list, const std::string& word)
  {
    auto it = list.begin();
    for (; it != list.end(); it++)
    {
      if (*it == word)
      {
        return it;
      }
    }
    return it;
  }
}

void alymova::create(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  if (set.find(name) != set.end())
  {
    out << "<ALREADY CREATED>";
    return;
  }
  Dictionary dict;
  set.emplace(name, dict);
  out << "<SUCCESSFULLY CREATED>";
}

void alymova::size(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID ERROR>");
  }
  auto it = set.find(name);
  if (it == set.end())
  {
    out << "<NOT FOUND>";
    return;
  }
  if (it->second.size() == 0)
  {
    out << "<EMPTY>";
    return;
  }
  out << it->second.size();
}

void alymova::removeDict(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto it = set.find(name);
  if (it == set.end())
  {
    out << "<NOT FOUND>";
    return;
  }
  set.erase(name);
  out << "<SUCCESSFULLY REMOVED>";
}

void alymova::addWord(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, word, translate;
  in >> name >> word >> translate;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto it_dict = set.find(name);
  if (it_dict == set.end())
  {
    out << "<NOT FOUND>";
    return;
  }
  Dictionary& dict = it_dict->second;
  auto it_word = dict.find(word);
  if (it_word == dict.end())
  {
    std::list< std::string > translates{translate};
    dict.emplace(word, translates);
    out << "<ADDED WORD AND TRANSLATE>";
    return;
  }
  TranslateSet& translates = it_word->second;
  if (findTranslate(translates, translate) == translates.cend())
  {
    translates.push_back(translate);
    out << "<ADDED TRANSLATE>";
    return;
  }
  out << "<WORD AND TRANSLATE ALREADY ADDED>";
}

void alymova::fixWord(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, word, new_word;
  in >> name >> word >> new_word;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto it_dict = set.find(name);
  if (it_dict == set.end())
  {
    out << "<NOT FOUND>";
    return;
  }
  Dictionary& dict = it_dict->second;
  auto it_word = dict.find(word);
  if (it_word == dict.end())
  {
    out << "<NOT FOUND>";
    return;
  }
  dict[new_word] = dict[word];
  dict.erase(word);
  out << "<SUCCESSFULLY FIXED>";
}

void alymova::findWord(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name, word;
  in >> name >> word;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto it_dict = set.find(name);
  if (it_dict == set.end())
  {
    out << "<NOT FOUND>";
    return;
  }
  const Dictionary& dict = it_dict->second;
  auto it_word = dict.find(word);
  if (it_word == dict.end())
  {
    out << "<NOT FOUND>";
    return;
  }
  out << it_word->second;
}

void alymova::containSubword(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, subword;
  in >> name >> subword;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto it_dict = set.find(name);
  if (it_dict == set.end())
  {
    out << "<NOT FOUND>";
    return;
  }
  Dictionary& dict = it_dict->second;
  std::list< std::string > suitable;
  for (auto it = dict.begin(); it != dict.end(); it++)
  {
    if (it->first.find(subword) != std::string::npos)
    {
      suitable.push_back(it->first);
    }
  }
  if (suitable.empty())
  {
    out << "<NOT FOUND>";
  }
  out << suitable;
}

void alymova::removeWord(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, word;
  in >> name >> word;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto it_dict = set.find(name);
  if (it_dict == set.end())
  {
    out << "<NOT FOUND>";
    return;
  }
  Dictionary& dict = it_dict->second;
  size_t cnt = dict.erase(word);
  if (cnt == 0)
  {
    out << "<NOT FOUND>";
    return;
  }
  out << "<SUCCESSFULLY REMOVED>";
}

void alymova::addTranslate(std::istream& in, std::ostream& out, DictSet& set)
{
  addWord(in, out, set);
}

void alymova::findEnglishEquivalent(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto it_dict = set.find(name);
  if (it_dict == set.end())
  {
    out << "<NOT FOUND>";
    return;
  }

  std::list< std::string > translates, equivalents;
  in >> translates;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Dictionary& dict = it_dict->second;
  for (auto it = dict.begin(); it != dict.end(); it++)
  {
    const TranslateSet& old_tr = it->second;
    for (auto it_list = translates.begin(); it_list != translates.end(); it_list++)
    {
      if (findTranslate(old_tr, *it_list) != old_tr.cend())
      {
        equivalents.push_back(it->first);
        break;
      }
    }
  }
  if (equivalents.empty())
  {
    out << "<NOT FOUND>";
    return;
  }
  out << equivalents;
}

void alymova::removeTranslate(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, word, translate;
  in >> name >> word >> translate;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto it_dict = set.find(name);
  if (it_dict == set.end())
  {
    out << "<NOT FOUND>";
    return;
  }
  Dictionary& dict = it_dict->second;
  auto it_word = dict.find(word);
  if (it_word == dict.end())
  {
    out << "<NOT FOUND>";
    return;
  }
  auto it_translate = findTranslate(it_word->second, translate);
  if (it_translate == it_word->second.cend())
  {
    out << "<NOT FOUND>";
    return;
  }
  it_word->second.erase(it_translate);
  out << "<SUCCESSFULLY REMOVED>";
}

void alymova::printContent(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  auto it_dict = set.find(name);
  if (it_dict == set.end())
  {
    out << "<NOT FOUND>";
    return;
  }
  const Dictionary& dict = it_dict->second;
  if (dict.empty())
  {
    out << '\n';
    return;
  }
  char c = dict.begin()->first[0];
  out << c;
  for (auto it = dict.begin(); it != dict.end(); it++)
  {
    if (it->first[0] == c)
    {
      out << ' ' << it->first;
    }
    else
    {
      c = it->first[0];
      out << '\n' << c << ' ' << it->first;
    }
  }
}

void alymova::translate(std::istream& in, std::ostream& out, DictSet& set)
{
  std::list< std::string > names;
  std::string word;
  in >> names >> word;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  std::list< Dictionary > dicts;
  for (auto it = names.begin(); it != names.end(); it++)
  {
    auto it_dict = set.find(*it);
    if (it_dict == set.end())
    {
      out << "<NOT FOUND>";
      return;
    }
    dicts.push_back(it_dict->second);
  }
  std::list< std::string > translates;
  for (auto it = dicts.begin(); it != dicts.end(); it++)
  {
    auto it_word = it->find(word);
    if (it_word != it->end())
    {
      translates.insert(translates.end(), it_word->second.begin(), it_word->second.end());
    }
  }
  if (translates.empty())
  {
    out << "<NOT FOUND>";
    return;
  }
  translates.sort();
  translates.unique();
  out << translates;
}
