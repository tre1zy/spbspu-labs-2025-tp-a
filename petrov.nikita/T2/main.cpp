#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "data_struct.hpp"

int main()
{
  using namespace petrov;
  std::vector< DataStruct > data;
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
}