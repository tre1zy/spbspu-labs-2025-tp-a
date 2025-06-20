#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "DataStruct.hpp"
#include <sstream>

int main()
{
  using ageev::DataStruct;

  std::vector< DataStruct > data;
  std::istringstream iss("(:key1 0.0d:key2 1234.0e-2:key3 \"Data\":)");


  std::copy(
    std::istream_iterator< DataStruct >(std::cin),
    std::istream_iterator< DataStruct >(),
    std::back_inserter(data)
  );

  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );

  return 0;
}
