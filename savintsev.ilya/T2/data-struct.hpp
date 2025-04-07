#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <iostream>
#include <cstddef>
#include <string>

namespace savintsev
{
  //DBL SCI | ULL OCT
  struct DataStruct
  {
    double key1;
    size_t key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct DoubleIO
  {
    double & ref;
  };

  struct UllIO
  {
    size_t & ref;
  };

  struct StringIO
  {
    std::string & ref;
  };

  struct LabelIO
  {
    std::string & ref;
  };

  std::istream & operator>>(std::istream & in, DelimiterIO && dest);
  std::istream & operator>>(std::istream & in, DoubleIO && dest);
  std::istream & operator>>(std::istream & in, StringIO && dest);
  std::istream & operator>>(std::istream & in, UllIO && dest);
  std::istream & operator>>(std::istream & in, LabelIO && dest);
  std::istream & operator>>(std::istream & in, DataStruct & dest);
  std::ostream & operator<<(std::ostream & out, const DataStruct & dest);
}

#endif
