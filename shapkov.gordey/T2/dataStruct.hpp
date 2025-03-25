#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include "types.hpp"
#include <iostream>

namespace shapkov
{
  struct DataStruct
  {
    doubleScientificIO key1;
    ratioIO key2;
    stringIO key3;
  };
  //std::istream& operator>>(std::istream& in, DataStruct& rhs);
  std::ostream& operator<<(std::ostream& out, DataStruct& rhs);
}
#endif
