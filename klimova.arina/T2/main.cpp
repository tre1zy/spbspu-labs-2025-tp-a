#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include "datastruct.hpp"

int main()
{
  using nspace::DataStruct;
  std::vector<Data> data;
  std::copy
  (
    std::istream_iterator<DataStruct>(std::cin),
    std::istream_iterator<DataStruct>(),
    std::back_inserter(data)
  );
  std::sort(data.begin(), data.end(), nspace::compareData);
  std::copy
  (
    data.begin(),
    data.end(),
    std::ostream_iterator<Data>(std::cout, "\n")
  );

return 0;
}
