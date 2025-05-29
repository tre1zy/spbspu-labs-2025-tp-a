#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>
#include <iterator>

#include "dataStruct.hpp"

int main()
{
  using namespace ivanova;
  using iIterator = std::istream_iterator<dataStruct>;
  using oIterator = std::ostream_iterator<dataStruct>;

  std::vector<dataStruct> data;
  std::copy(iIterator(std::cin), iIterator(), std::back_inserter(data));

  while (!std::cin.eof() && std::cin.fail())
  {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::copy(iIterator(std::cin), iIterator(), std::back_inserter(data));
  }

  std::sort(data.begin(), data.end(), compareDataStructs);
  std::copy(data.begin(), data.end(), oIterator(std::cout, "\n"));
  return 0;
}