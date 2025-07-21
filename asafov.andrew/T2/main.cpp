#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include "datastruct.h"

int main()
{
  using asafov::DataStruct;

  std::vector<DataStruct> data;
  std::copy(
    std::istream_iterator<DataStruct>{std::cin},
    std::istream_iterator<DataStruct>{},
    std::back_inserter(data)
  );

  std::sort(data.begin(), data.end(), asafov::cmpDataStruct);
  std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>{std::cout, "\n"});

  return 0;
}
