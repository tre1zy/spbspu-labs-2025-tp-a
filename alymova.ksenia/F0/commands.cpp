#include "commands.hpp"
#include <algorithm>
#include <iostream>
#include <exception>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>
#include <iterator>
#include "dict-utils.hpp"

namespace
{
  using namespace alymova;

  WordSet::const_iterator findTranslate(const WordSet& list, const std::string& word)
  {
    return std::find(list.begin(), list.end(), word);
  }
  bool hasSubword(const std::string& word, const std::string& subword)
  {
    return word.find(subword) != std::string::npos;
  }
  bool hasTranslate(WordSet& list, const std::string& word)
  {
    return std::find(list.begin(), list.end(), word) != list.end();
  }
  bool hasAnyTranslates(const std::pair< std::string, WordSet >& pair, WordSet& translates)
  {
    return std::any_of(translates.begin(), translates.end(), std::bind(hasTranslate, pair.second, _1));
  }
  bool compareKeys(const std::pair< std::string, WordSet>& pair1, const std::pair< std::string, WordSet>& pair2)
  {
    return pair1.first < pair2.first;
  }
  template< class T >
  std::string returnName(const std::pair< std::string, T >& p)
  {
    return p.first;
  }
  std::pair< std::string, WordSet > unionLists(const std::pair< std::string, WordSet >& pair,
    const Dictionary& unioned)
  {
    WordSet list1 = pair.second;
    WordSet list2 = unioned.at(pair.first);
    list1.sort();
    list2.sort();
    WordSet res;
    std::set_union(list1.begin(), list1.end(), list2.begin(), list2.end(), std::back_inserter(res));
    return {pair.first, res};
  }
  std::pair< std::string, WordSet > intersectLists(const std::pair< std::string, WordSet >& pair,
    const Dictionary& intersected)
  {
    WordSet list1 = pair.second;
    WordSet list2 = intersected.at(pair.first);
    list1.sort();
    list2.sort();
    WordSet res;
    std::set_intersection(list1.begin(), list1.end(), list2.begin(), list2.end(), std::back_inserter(res));
    return {pair.first, res};
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

void alymova::dicts(std::ostream& out, const DictSet& set)
{
  if (set.empty())
  {
    return;
  }
  WordSet names;
  std::transform(set.begin(), set.end(), std::back_inserter(names), returnName< Dictionary >);
  out << names;
}

void alymova::size(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID ERROR>");
  }
  const Dictionary& dict = set.at(name);
  if (dict.size() == 0)
  {
    out << "<EMPTY>";
    return;
  }
  out << dict.size();
}

void alymova::removeDict(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  set.at(name);
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
  Dictionary& dict = set.at(name);
  auto it_word = dict.find(word);
  if (it_word == dict.end())
  {
    WordSet translates{translate};
    dict.emplace(word, translates);
    out << "<WORD AND TRANSLATE WERE ADDED>";
    return;
  }
  WordSet& translates = it_word->second;
  if (findTranslate(translates, translate) == translates.cend())
  {
    translates.push_back(translate);
    out << "<TRANSLATE WAS ADDED>";
    return;
  }
  out << "<WORD AND TRANSLATE ALREADY WERE ADDED>";
}

void alymova::fixWord(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, word, new_word;
  in >> name >> word >> new_word;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Dictionary& dict = set.at(name);
  dict[new_word] = dict.at(word);
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
  const Dictionary& dict = set.at(name);
  out << dict.at(word);
}

void alymova::containSubword(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name, subword;
  in >> name >> subword;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Dictionary& dict = set.at(name);
  WordSet words, suitable;
  std::transform(dict.begin(), dict.end(), std::back_inserter(words), returnName< WordSet >);
  std::copy_if(words.begin(), words.end(), std::back_inserter(suitable), std::bind(hasSubword, _1, subword));
  if (suitable.empty())
  {
    out << "<NOT FOUND>";
    return;
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
  Dictionary& dict = set.at(name);
  dict.at(word);
  dict.erase(word);
  out << "<SUCCESSFULLY REMOVED>";
}

void alymova::addTranslate(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, word, translate;
  in >> name >> word >> translate;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Dictionary& dict = set.at(name);
  WordSet& translates = dict.at(word);
  if (findTranslate(translates, translate) != translates.cend())
  {
    out << "<TRANSLATE WAS ALREADY ADDED>";
    return;
  }
  translates.push_back(translate);
  out << "<TRANSLATE WAS ADDED>";
}

void alymova::findEnglishEquivalent(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name;
  WordSet translates;
  in >> name >> translates;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Dictionary& dict = set.at(name);
  Dictionary equivalents;
  auto f = std::bind(hasAnyTranslates, _1, translates);
  std::copy_if(dict.begin(), dict.end(), std::inserter(equivalents, equivalents.end()), f);
  WordSet words;
  std::transform(equivalents.begin(), equivalents.end(), std::back_inserter(words), returnName< WordSet >);
  if (words.empty())
  {
    out << "<NOT FOUND>";
    return;
  }
  out << words;
}

void alymova::removeTranslate(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string name, word, translate;
  in >> name >> word >> translate;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  Dictionary& dict = set.at(name);
  WordSet& translates = dict.at(word);
  auto it = findTranslate(translates, translate);
  if (it == translates.cend())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  translates.erase(it);
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
  const Dictionary& dict = set.at(name);
  if (dict.empty())
  {
    return;
  }
  auto it = dict.begin();
  char first_letter = it->first[0];
  out << first_letter << ' ' << (it++)->first;
  for (; it != dict.end(); it++)
  {
    if (it->first[0] == first_letter)
    {
      out << ' ' << it->first;
    }
    else
    {
      first_letter = it->first[0];
      out << '\n' << first_letter << ' ' << it->first;
    }
  }
}

void alymova::translate(std::istream& in, std::ostream& out, const DictSet& set)
{
  WordSet names;
  std::string word;
  in >> names >> word;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::list< Dictionary > dicts;
  for (auto it = names.begin(); it != names.end(); it++)
  {
    dicts.push_back(set.at(*it));
  }
  WordSet translates;
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

void alymova::unionDicts(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string newname, name1, name2;
  in >> newname >> name1 >> name2;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Dictionary& dict1 = set.at(name1);
  const Dictionary& dict2 = set.at(name2);
  Dictionary difference, tmp, unioned;

  auto d_first = std::inserter(difference, difference.end());
  std::set_difference(dict1.begin(), dict1.end(), dict2.begin(), dict2.end(), d_first, compareKeys);

  d_first = std::inserter(tmp, tmp.end());
  std::set_intersection(dict1.begin(), dict1.end(), dict2.begin(), dict2.end(), d_first, compareKeys);
  std::transform(tmp.begin(), tmp.end(), std::inserter(unioned, unioned.end()), std::bind(unionLists, _1, dict2));

  unioned.insert(difference.begin(), difference.end());
  set[newname] = unioned;
  out << "<SUCCESSFULLY INTERSECTED>";
}

void alymova::intersectDicts(std::istream& in, std::ostream& out, DictSet& set)
{
  std::string newname, name1, name2;
  in >> newname >> name1 >> name2;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Dictionary& dict1 = set.at(name1);
  const Dictionary& dict2 = set.at(name2);
  Dictionary tmp, intersected;

  auto d_first = std::inserter(tmp, tmp.end());
  std::set_intersection(dict1.begin(), dict1.end(), dict2.begin(), dict2.end(), d_first, compareKeys);

  d_first = std::inserter(intersected, intersected.end());
  std::transform(tmp.begin(), tmp.end(), d_first, std::bind(intersectLists, _1, dict2));

  set[newname] = intersected;
  out << "<SUCCESSFULLY INTERSECTED>";
}

void alymova::printDayWord(std::istream& in, std::ostream& out, const DictSet& set)
{
  std::string name;
  in >> name;
  if (!in)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  const Dictionary& dict = set.at(name);
  if (dict.empty())
  {
    out << "Sorry, today is a bad day";
    return;
  }
  std::srand(std::time(nullptr));
  int random_num = std::rand() % dict.size();
  auto it_word = dict.begin();
  for (int i = 0; i < random_num; i++, it_word++);
  out << "Have a good day with word:\n";
  out << it_word->first << ' ' << it_word->second;
}

void alymova::printHelp(std::ostream& out)
{
  out << std::setw(60) << std::left << "create <dict> " << "create new empty dictionary\n";
  out << std::setw(60) << std::left << "dictionary_list" << "print list of current dictionaries\n";
  out << std::setw(60) << std::left << "size <name> " << "print dictionary size\n";
  out << std::setw(60) << std::left << "remove_dictionary <name> " << "remove existing dictionary\n";
  out << std::setw(60) << std::left << "add_word <dict> <word> <translate> ";
  out << "add word with translate in dictionary\n";
  out << std::setw(60) << std::left << "fix_word <dict> <word> <new_word> ";
  out << "replace word in dictionary by new_word\n";
  out << std::setw(60) << std::left << "find_word <dict> <word> ";
  out << "print word translates from dictionary\n";
  out << std::setw(60) << std::left << "contain <dict> <subword> ";
  out << "print words that contain subword in dictionary\n";
  out << std::setw(60) << std::left << "remove_word <dict> <word> ";
  out << "remove existing word from existing dictionary\n";
  out << std::setw(60) << std::left << "add_translate <dict> <word> <translate> ";
  out << "add new translate to existing word in dictionary\n";
  out << std::setw(60) << std::left << "find_english_equivalent <dict> <tr_count> <tr_1> ... <tr_n> ";
  out << "find english equivalents of translates in dictionary\n";
  out << std::setw(60) << std::left << "remove_translate <dict> <word> <translate> ";
  out << "remove existing translate of existing word in existing dictionary\n";
  out << std::setw(60) << std::left << "print_content <dict> ";
  out << "print dictionary content by letters\n";
  out << std::setw(60) << std::left << "translate <dict_count> <dict_1> ... <dict_n> <word> ";
  out << "print word translates from dictionaries\n";
  out << std::setw(60) << std::left << "union <dict1> <dict2> ";
  out << "create new dictionary as union of dictionary1, dictionary2\n";
  out << std::setw(60) << std::left << "intersect <dict1> <dict2> ";
  out << "create new dictionary as intersection of dictionary1, dictionary2\n";
  out << std::setw(60) << std::left << "day_word <dict> ";
  out << "print word of day:)\n";
  out << "\nIf dictionaries were load from file, result of programm will be saved in this file.\n";
  out << "In the opposite case result of programm will be saved in file \"dictionaries\".";
}
