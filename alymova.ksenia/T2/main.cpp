#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include "keys.hpp"

int main()
{
  using namespace alymova;
  StreamGuard os_sg(std::cout);
  StreamGuard is_sg(std::cin);

  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    DataStruct d;
    std::cin >> d;
    data.push_back(d);
    if (std::cin.fail())
    {
      data.pop_back();
      std::cin.clear(std::cin.rdstate() ^ std::ios_base::failbit);
    }
  }
  /*while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(data)
    );
    if (std::cin.fail())
    {
      data.pop_back();
      std::cin.clear(std::cin.rdstate() ^ std::ios_base::failbit);
    }
  }*/
  std::cout << data.size();
  std::sort(data.begin(), data.end(), comparator);
  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
}
