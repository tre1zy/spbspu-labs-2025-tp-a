#include <vector>
#include <limits>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "dataStruct.hpp"

int main()
{
  using namespace duhanina;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    DataStruct temp;
    if (std::cin >> temp)
    {
      data.push_back(temp);
    }
    else
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  if (data.empty())
  {
    std::cerr << "empty\n";
    return 1;
  }
  std::sort(data.begin(), data.end());
  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
  return 0;
}
