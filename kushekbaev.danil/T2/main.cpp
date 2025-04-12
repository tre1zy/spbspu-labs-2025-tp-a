#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include "streamguard.hpp"
#include "datastruct.hpp"

int main()
{
  kushekbaev::StreamGuard outsg(std::cout);
  kushekbaev::StreamGuard insg(std::cin);

  std::vector< kushekbaev::DataStruct > data;
  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
    }
    std::copy(std::istream_iterator< kushekbaev::DataStruct >(std::cin),
      std::istream_iterator< kushekbaev::DataStruct >(), std::back_inserter(data));
  }

  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator< kushekbaev::DataStruct >(std::cout, "\n"));
}
