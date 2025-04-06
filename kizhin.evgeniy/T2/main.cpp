#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "io-utils.hpp"

int main()
{
  using namespace kizhin;
  using InputIt = std::istream_iterator< DataStruct >;
  using DataStructContainer = std::vector< DataStruct >;
  try {
    DataStructContainer vals(InputIt(std::cin), InputIt{});
    while (!std::cin.eof()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      vals.insert(vals.end(), InputIt(std::cin), InputIt{});
    }
    std::sort(vals.begin(), vals.end());
    std::ostream_iterator< DataStruct > outIt(std::cout, "\n");
    std::copy(vals.begin(), vals.end(), outIt);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}

