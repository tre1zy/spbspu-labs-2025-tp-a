#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "data_struct.hpp"

namespace petrov
{
  void sort(std::vector< DataStruct > & vector);
}

int main()
{
  using namespace petrov;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(
    std::istream_iterator< DataStruct >(std::cin),
    std::istream_iterator< DataStruct >(),
    std::back_inserter(data)
    );
    std::cin.clear();
    std::cin.ignore(1024, '\n');
  }
  std::copy(
      std::begin(data),
      std::end(data),
      std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
}

void petrov::sort(std::vector< DataStruct > & vector)
{
  bool swapped = false;
}
