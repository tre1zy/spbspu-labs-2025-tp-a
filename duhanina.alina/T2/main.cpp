#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "dataStruct.hpp"

int main()
{
  using namespace duhanina;
  std::vector< DataStruct > data;
  std::copy(
    std::istream_iterator< DataStruct >(std::cin),
    std::istream_iterator< DataStruct >(),
    std::back_inserter(data)
  );
  std::sort(data.begin(), data.end());
  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
  return 0;
}
