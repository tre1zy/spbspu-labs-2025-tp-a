#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "data_struct.hpp"

int main()
{
  using InputStreamIterator = std::istream_iterator<sherkunov::DataStruct>;
  using OutputStreamIterator = std::ostream_iterator<sherkunov::DataStruct>;

  std::vector<sherkunov::DataStruct> data;

  std::copy(InputStreamIterator(std::cin), InputStreamIterator{}, std::back_inserter(data));

  if (std::cin.bad()) {
    std::cerr << "Error: Critical input failure\n";
    return 1;
  }

  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), OutputStreamIterator(std::cout, "\n"));

  return 0;
}
