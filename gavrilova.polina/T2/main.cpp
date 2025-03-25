#include <iostream>
#include <vector>
#include <iterator>
#include <limits>

#include "DataStruct.hpp"

int main()
{
  using namespace gavrilova;
  std::vector< DataStruct > data;

  DataStruct new_struct;
  while (std::cin >> new_struct && !std::cin.eof()) {
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      data.push_back(new_struct);
    }
  }
  // std::copy(
  //   std::istream_iterator< DataStruct >(std::cin),
  //   std::istream_iterator< DataStruct >(),
  //   std::back_inserter(data)
  // );
  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
}
