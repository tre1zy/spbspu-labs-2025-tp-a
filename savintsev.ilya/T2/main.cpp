#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "data-struct.hpp"

int main()
{
  using namespace savintsev;

  std::vector< DataStruct > dataVec;

  std::istream_iterator< DataStruct > start(std::cin);
  std::istream_iterator< DataStruct > end;

  std::copy(start, end, std::back_inserter(dataVec));

  std::sort(dataVec.begin(), dataVec.end(), [](const DataStruct &a, const DataStruct &b)
  {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
  });

  std::copy(std::begin(dataVec), std::end(dataVec), std::ostream_iterator< DataStruct >(std::cout, "\n"));
}
