#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include "stream_guard.hpp"
#include "datastruct.hpp"

int main()
{
  using namespace kazak;
  using kazak::DataStruct;

  std::vector< DataStruct > data;

  std::copy(
    std::istream_iterator< DataStruct >(std::cin),
    std::istream_iterator< DataStruct >(),
    std::back_inserter(data)
  );

  std::sort(data.begin(), data.end());

  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );

  return 0;
}

