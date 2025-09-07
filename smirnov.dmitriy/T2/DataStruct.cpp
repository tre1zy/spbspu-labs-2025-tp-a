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
