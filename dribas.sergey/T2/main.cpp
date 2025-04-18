#include <algorithm>
#include <vector>
#include <limits>
#include <iterator>
#include <iostream>

#include "data.hpp"

int main() {
  using namespace dribas;

  std::vector< Data > data;

  while(true) {
    std::copy(
    std::istream_iterator< Data >(std::cin),
    std::istream_iterator< Data >(),
    std::back_inserter(data)
    );

    if (std::cin.eof()) {
      break;
    }

    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end(), compare);
  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator<Data>(std::cout)
  );

  return 0;
}
