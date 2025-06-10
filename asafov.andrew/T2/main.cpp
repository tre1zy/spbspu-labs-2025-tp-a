#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <limits>
#include "datastruct.h"

int main()
{
  std::vector< asafov::DataStruct > data;

  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator< asafov::DataStruct >{ std::cin },
      std::istream_iterator< asafov::DataStruct >{},
      std::back_inserter(data)
    );

    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(
    data.begin(),
    data.end(),
    asafov::cmpDataStruct
  );

  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator< asafov::DataStruct >{std::cout, "\n"}
  );
  return 0;
}
