#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include "datastruct.hpp"
#include "stream_guard.hpp"

int main()
{
  using namespace kazak;
  using DataStructs = std::vector< DataStruct >;

  DataStructs data;

  std::copy(std::istream_iterator< DataStruct >(std::cin),
            std::istream_iterator< DataStruct >(),
            std::back_inserter(data));

  std::sort(data.begin(), data.end());

  std::copy(data.begin(), data.end(),
            std::ostream_iterator< DataStruct >(std::cout, "\n"));

  return 0;
}

