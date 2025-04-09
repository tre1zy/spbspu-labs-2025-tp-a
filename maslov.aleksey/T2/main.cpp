#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include "dataStruct.hpp"

int main()
{
  using namespace maslov;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy
    (
      std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
    }
  }
  std::sort(data.begin(), data.end());
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
}
