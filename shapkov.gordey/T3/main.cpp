#include <iostream>
#include <vector>
#include <iterator>
#include <limits>
#include "GeometricalTypes.hpp"

int main()
{
  using shapkov::Polygon;
  std::vector< Polygon > data;
  using inputIt = std::istream_iterator< Polygon >;
  using outputIt = std::ostream_iterator< Polygon >;
  while (!std::cin.eof())
  {
    std::copy(inputIt(std::cin), inputIt(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  std::copy(std::begin(data), std::end(data), outputIt(std::cout, "\n"));
  return 0;
}
