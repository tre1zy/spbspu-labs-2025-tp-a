#include <iostream>
#include <vector>
#include "data-struct.hpp"
#include <iterator>
#include <algorithm>

int main()
{
  using namespace belyaev;

  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator< DataStruct >{std::cin},
      std::istream_iterator< DataStruct >{},
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
    }
  }

  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator< DataStruct >{std::cout, "\n"});

  return 0;
}
