#include "data_struct.hpp"
#include <vector>
#include <iterator>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

int main()
{
  std::vector< demehin::DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator< demehin::DataStruct >(std::cin),
      std::istream_iterator< demehin::DataStruct >(),
      std::back_inserter(data)
    );

    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());

  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< demehin::DataStruct >(std::cout, "\n")
  );
}


