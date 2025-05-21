#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "DataStruct.hpp"

int main()
{
  using namespace holodilov;
  using is_iterator = std::istream_iterator< DataStruct >;
  using os_iterator = std::ostream_iterator< DataStruct >;
  std::vector< DataStruct > vec;
  while (!std::cin.eof())
  {
    std::copy(is_iterator(std::cin), is_iterator(), std::back_inserter(vec));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(vec.begin(), vec.end());
  std::copy(std::begin(vec), std::end(vec), os_iterator(std::cout, "\n"));
  return 0;
}
