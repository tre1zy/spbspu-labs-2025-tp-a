#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <ios>
#include <string>

namespace kushekbaev
{
  struct DataStruct
  {
    unsigned long long key1;
    char key2;
    std::string key3;
    bool operator<(const DataStruct& rhs) const;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct ULLIO
  {
    unsigned long long& ref;
  };

  struct CharIO
  {
    char& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, ULLIO&& dest);
  std::istream& operator>>(std::istream& in, CharIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
}

#endif
