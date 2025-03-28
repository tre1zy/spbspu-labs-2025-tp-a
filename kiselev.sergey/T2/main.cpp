#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include "data.hpp"
int main()
{
  std::vector< kiselev::DataStruct > data;
  while(!std::cin.eof())
  {
    std::copy
    (
      std::istream_iterator< kiselev::DataStruct >(std::cin),
      std::istream_iterator< kiselev::DataStruct >(),
      std::back_inserter(data)
    );
  }
  std::sort(data.begin(), data.end(), kiselev::compare);
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< kiselev::DataStruct >(std::cout, "\n")
  );
  return 0;
}
