#include "dataStruct.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

int main() {
  std::vector<kostyukov::DataStruct> data;
  while (!std::cin.eof()) {
    std::copy(std::istream_iterator<kostyukov::DataStruct>(std::cin),
              std::istream_iterator<kostyukov::DataStruct>(),
              std::back_inserter(data));
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), kostyukov::comparator);
  std::cout << "Data:\n";
  std::copy(std::begin(data), std::end(data),
            std::ostream_iterator<kostyukov::DataStruct>(std::cout, "\n"));
  return 0;
}
