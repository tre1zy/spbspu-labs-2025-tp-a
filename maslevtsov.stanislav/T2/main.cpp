#include <vector>
#include <iostream>
#include <iterator>
#include <limits>
#include <algorithm>
#include "data_struct.hpp"
#include "comparator.hpp"

int main()
{
  std::vector< maslevtsov::DataStruct > data;

  while (!std::cin.eof()) {
    std::copy(std::istream_iterator< maslevtsov::DataStruct >(std::cin),
      std::istream_iterator< maslevtsov::DataStruct >(), std::back_inserter(data));
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end(), maslevtsov::comparator);

  std::copy(std::begin(data), std::end(data), std::ostream_iterator< maslevtsov::DataStruct >(std::cout, "\n"));
}
