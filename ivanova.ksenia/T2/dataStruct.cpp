#include "dataStruct.hpp"

bool compareDataStructs(const ivanova::dataStruct& a, const ivanova::dataStruct& b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  else if (a.key2 != b.key2)
  {
    return a.key2 < b.key2;
  }
  else
  {
    return a.key3.length() < b.key3.length();
  }
}

