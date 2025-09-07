#include "DataStruct.hpp"
#include <bitset>
#include <algorithm>
#include "Delimiter.hpp"
#include "ScopeGuard.hpp"

std::string getBinNumber(unsigned long long value)
{
  std::bitset< 64 > bin(value);
  std::string binString = bin.to_string();
  return "0" + binString.erase(0, binString.find('1'));
}

bool smirnov::DataStruct::operator<(const DataStruct& other) const
{
  if (key1 != other.key1)
  {
    return key1 < other.key1;
  }

  else if (key2 != other.key2)
  {
    return key2 < other.key2;
  }

  return (key3.length() <= other.key3.length());
}
