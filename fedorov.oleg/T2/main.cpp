#include "data_struct.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
  using namespace fedorov;

  std::vector<DataStruct> data;
  DataStruct temp;
  while ( std::cin >> temp ) {
    data.push_back(temp);
    std::cin.clear();
  }
  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>{std::cout, "\n"});

  return 0;
}