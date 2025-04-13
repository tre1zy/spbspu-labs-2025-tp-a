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
  std::string input = "(:key2 (:N -1:D 7:):key1 0xFC92D:key3 \"data\":)";

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

  std::cout << "Data:\n";
  std::copy(std::begin(data), std::end(data), std::ostream_iterator< DataStruct >(std::cout, "\n"));

  return 0;
}
