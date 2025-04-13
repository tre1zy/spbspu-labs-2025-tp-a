#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <limits>
#include <iomanip>

#include "data_struct.hpp"

int main()
{
  std::vector< rychkov::DataStruct > values;
  while (!std::cin.eof())
  {
    using Iter = std::istream_iterator< rychkov::DataStruct >;
    std::copy(Iter(std::cin), Iter(), std::back_inserter(values));
    if (!std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::cout << std::setprecision(1);
  for (const rychkov::DataStruct& i: values)
  {
    std::cout << i << '\n';
  }
}
