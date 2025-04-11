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

  std::istream & operator>>(std::istream & in, DataStruct & dest);
  std::ostream & operator<<(std::ostream & out, const DataStruct & dest);
}

#endif
