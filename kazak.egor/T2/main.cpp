#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <iterator>
#include <vector>
#include <iomanip>
#include "stream_guard.hpp"
#include "datastruct.hpp"

int main()
{
  using namespace kazak;
  using kazak::DataStruct;

  std::vector< DataStruct > data;
  std::string input;

  while (true)
  {
    std::getline(std::cin, input);

    if (input.empty())
    {
        break;
    }

    std::istringstream iss(input);

    while (iss >> std::ws)
    {
      DataStruct ds;
      if (iss >> ds)
      {
        data.push_back(ds);
      }
      else
      {
        iss.clear();
        break;
      }
    }
  }

  std::copy(std::begin(data), std::end(data), std::ostream_iterator<DataStruct>(std::cout, "\n"));

  return 0;
}
