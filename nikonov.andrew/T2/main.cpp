#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "DataStruct.hpp"
int main()
{
  using nikonov::DataStruct;
  std::vector< DataStruct > dS;
  std::copy
  (
    std::istream_iterator< DataStruct >(std::cin),
    std::istream_iterator< DataStruct >(),
    std::back_inserter(dS)
  );
}
