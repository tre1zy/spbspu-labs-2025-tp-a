#include <algorithm>
#include <iostream>
#include <limits>
#include <iterator>
#include <vector>
#include "data.h"
int main()
{
  using istreamIter = std::istream_iterator< horoshilov::DataStruct >;
  using ostreamIter = std::ostream_iterator< horoshilov::DataStruct >;
  using dataSt = horoshilov::DataStruct;

  std::vector< dataSt > data;
  while (!std::cin.eof())
  {
    std::copy(istreamIter(std::cin),istreamIter(),std::back_inserter(data));

    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end(), horoshilov::compareData);
  std::copy(std::begin(data),std::end(data),ostreamIter(std::cout, "\n"));
  return 0;
}