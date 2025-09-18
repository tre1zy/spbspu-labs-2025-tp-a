#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include "DataStruct.hpp"

int main()
{
  using ageev::DataStruct;

  std::vector< DataStruct > data;

  using InputDataIt = std::istream_iterator< DataStruct >;

  while (!std::cin.eof())
  {
    std::copy(InputDataIt(std::cin), InputDataIt(), std::back_inserter(data));
    if (std::cin.fail() && !std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    if(std::cin.badbit)
    { 
      std::cout << "программа не может продолжить работу";
      return 2;
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator< DataStruct >(std::cout, "\n"));
  return 0;
}
