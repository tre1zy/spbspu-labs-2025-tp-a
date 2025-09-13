#ifndef IO_UTILS_H
#define IO_UTILS_H
#include <list>
#include <map>

#include <iostream>

namespace orlova
{
  using Translations = std::list< std::string >;
  using Dictionary = std::map< std::string, Translations >;
  using Dictionaries = std::map< std::string, Dictionary >;

  std::istream& operator>>(std::istream&, Dictionary&);
  //std::ostream& operator<<(std::ostream&, const Dictionary&);
}

#endif
