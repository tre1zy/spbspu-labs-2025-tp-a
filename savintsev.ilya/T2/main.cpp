#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "data-struct.hpp"

int main()
{
  using namespace savintsev;

  std::vector< DataStruct > data;

  using data_iterator = std::istream_iterator< DataStruct >;
  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
    }
    std::copy(data_iterator(std::cin), data_iterator(), std::back_inserter(data));
  }

  std::sort(data.begin(), data.end(), [](const DataStruct & a, const DataStruct & b)
  {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
  });

  std::copy(data.begin(), data.end(), std::ostream_iterator< DataStruct >(std::cout, "\n"));
}
