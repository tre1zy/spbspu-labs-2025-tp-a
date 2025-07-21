#include <iostream>
#include <vector>
#include <algorithm>
#include "datastruct.h"

int main()
{
  using asafov::DataStruct;

  std::vector<DataStruct> data;
  DataStruct record;

  while (true)
  {
    std::streampos pos = std::cin.tellg();
    if (!(std::cin >> record))
    {
      if (std::cin.eof()) break;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
      data.push_back(record);
    }
  }

  std::sort(data.begin(), data.end(), asafov::cmpDataStruct);
  std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>{std::cout, "\n"});

  return 0;
}
