#include <algorithm>
#include <vector>
#include <limits>
#include <iterator>
#include <iostream>

#include "data.hpp"

int main()
{
  using namespace dribas;

  std::vector< Data > data;
  while (!std::cin.eof()) {
    Data temp;
    std::cin >> temp;
    if (std::cin) {
      data.push_back(temp);
    } else {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end(), compare);
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< Data >(std::cout, "\n")
  );

  return 0;
}
