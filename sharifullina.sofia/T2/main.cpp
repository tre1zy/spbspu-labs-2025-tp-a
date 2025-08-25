#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>

#include "dataStruct.hpp"

int main()
{
  using sharifullina::DataStruct;

  std::vector<DataStruct> data;
  std::copy(
    std::istream_iterator<DataStruct>(std::cin),
    std::istream_iterator<DataStruct>(),
    std::back_inserter(data)
  );

  if (!std::cin.eof() && std::cin.fail())
  {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));

  return 0;
}
