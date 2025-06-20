#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <functional>
#include <list>
#include <map>

namespace alymova
{
  using TranslateSet = std::list< std::string >;
  using Dictionary = std::map< std::string, TranslateSet >;
  using DictSet = std::map< std::string, Dictionary >;
  using CommandSet = std::map<
    std::string,
    std::function< void(DictSet&) >,
    std::less< std::string >
  >;

  void create(std::istream& in, DictSet& set);
  void size(std::istream& in, std::ostream& out, DictSet& set);
  void removeDict(std::istream& in, DictSet& set);
  void addWord(std::istream& in, DictSet& set);
  void fixWord(std::istream& in, DictSet& set);
  void findWord(std::istream& in, std::ostream& out, DictSet& set);
  void containSubword(std::istream& in, std::ostream& out, DictSet& set);
  void removeWord(std::istream& in, DictSet& set);
  void addTranslate(std::istream& in, DictSet& set);
  void findEnglishEquivalent(std::istream& in, std::ostream& out, DictSet& set);
  void removeTranslate(std::istream& in, DictSet& set);
  void printContent(std::istream& in, std::ostream& out, DictSet& set);
  void translate(std::istream& in, std::ostream& out, DictSet& set);
  void unionDicts(std::istream& in, DictSet& set);
  void intersectDicts(std::istream& in, DictSet& set);
  void printDayWord(std::istream& in, std::ostream& out, DictSet& set);

  void printHelp(std::ostream& out);
  DictSet readDictionaryFile(std::istream& in);
  void saveDictionaryFile(std::ostream& out);
}


#endif
