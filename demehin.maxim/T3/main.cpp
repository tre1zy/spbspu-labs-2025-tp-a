#include <iostream>
#include <limits>
#include <algorithm>
#include <iterator>
#include "geometry.hpp"


int main()
{
  using demehin::Polygon;
  using istrIter = std::istream_iterator< Polygon >;
  //using ostrIter = std::ostream_iterator< Polygon >;

  std::vector< Polygon > plgs;
  while (!std::cin.eof())
  {
    std::copy(istrIter(std::cin), istrIter(), std::back_inserter(plgs));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::cout << plgs.size() << "\n";
  std::cout << plgs[1].points[0].x << " " << plgs[1].points[0].y << "\n";
}
