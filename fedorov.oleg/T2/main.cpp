// Variant DBL LIT | CMP LSP

#include "data_struct.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
  using namespace fedorov;

  std::vector<DataStruct> data;

  std::copy(std::istream_iterator<DataStruct>{std::cin}, std::istream_iterator<DataStruct>{},
            std::back_inserter(data));

  data.erase(std::remove_if(data.begin(), data.end(),
                            [](const DataStruct &ds) { return std::cin.fail(); }),
             data.end());

  std::sort(data.begin(), data.end());

  std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>{std::cout, "\n"});

  return 0;
}
