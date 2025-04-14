#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "data_struct.hpp"
int main()
{
  std::vector< evstyunichev::DataStruct > data;
  while(!std::cin.eof())
  {
    std::copy
    (
      std::istream_iterator< evstyunichev::DataStruct >(std::cin),
      std::istream_iterator< evstyunichev::DataStruct >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), evstyunichev::comparator);
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< evstyunichev::DataStruct >(std::cout, "\n")
  );
  return 0;
}
