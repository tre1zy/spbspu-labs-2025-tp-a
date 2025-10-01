#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include "Dictionary.hpp"

namespace holodilov
{
  void createDict(std::istream& in, std::ostream& out, MapDictionaries& dictionaries);

  void exportDict(std::istream& in, std::ostream& out, MapDictionaries& dictionaries);

  void importDict(std::istream& in, std::ostream& out, MapDictionaries& dictionaries);

  void clearDict(std::istream& in, std::ostream& out, MapDictionaries& dictionaries);

  void deleteDict(std::istream& in, std::ostream& out, MapDictionaries& dictionaries);

  void addWord(std::istream& in, std::ostream& out, MapDictionaries& dictionaries);

  void deleteWord(std::istream& in, std::ostream& out, MapDictionaries& dictionaries);

  void updateWord(std::istream& in, std::ostream& out, MapDictionaries& dictionaries);

  void printDict(std::istream& in, std::ostream& out, const MapDictionaries& dictionaries);

  void findWord(std::istream& in, std::ostream& out, MapDictionaries& dictionaries);

  void merge(std::istream& in, std::ostream& out, MapDictionaries& dictionaries);

  void intersect(std::istream& in, std::ostream& out, MapDictionaries& dictionaries);

  void exportAlphabet(std::istream& in, std::ostream& out, const MapDictionaries& dictionaries);

  void checkAlphabet(std::istream& in, std::ostream& out, const MapDictionaries& dictionaries);

  void printDictNames(std::ostream& out, const MapDictionaries& dictionaries);
}

#endif
