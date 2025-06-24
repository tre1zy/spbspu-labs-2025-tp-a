#ifndef DICT_IMPORT_PARSERS_HPP
#define DICT_IMPORT_PARSERS_HPP

#include <vector>
#include <string>

#include "HashTable.hpp"

namespace crossref
{

  void removeWhitespace(std::string &str);
  std::vector< int > parseNumbers(const std::string &numbers_str);
  void processDictLine(HashTable &table, const std::string &line);

}

#endif
