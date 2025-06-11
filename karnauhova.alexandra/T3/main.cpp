#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "shapes.hpp"


int main(int argc, char** argv)
{
  using namespace karnauhova; 
  if (argc != 2)
  {
    std::cerr << "<INVALID ARGUMENTS>\n";
    return 1;
  }
  std::ifstream file;
  file.open(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "<INVALID FILE>\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  using istr_iterator = std::istream_iterator< Polygon >;
  using ostr_iter = std::ostream_iterator< Polygon >;
  while(!file.eof())
  {
    if (file.fail())
    {
        file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(istr_iterator(file), istr_iterator(), std::back_inserter(polygons));
  }

}
