#include "DataStruct.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

int main() {
  std::vector<DataStruct> dataVector;
  std::copy(std::istream_iterator<DataStruct>(std::cin), std::istream_iterator<DataStruct>(), std::back_inserter(dataVector));
  if (std::cin.fail() && !std::cin.eof()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  std::sort(dataVector.begin(), dataVector.end(), compareDataStruct);
  std::copy(dataVector.begin(), dataVector.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));
  return 0;
}
