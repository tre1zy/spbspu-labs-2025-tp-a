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

  std::vector< DataStruct > data{std::istream_iterator< DataStruct >(std::cin), std::istream_iterator< DataStruct >()};
  /*std::vector< DataStruct > data;
  std::istream_iterator< DataStruct > in(std::cin), eof;
  while (in != eof)
  {
    DataStruct d;
    d = *in++;
    if (!std::cin.fail())
    {
      data.push_back(d);
    }
    std::cin.clear(std::cin.rdstate() ^ std::ios_base::failbit);
  }*/
  std::sort(data.begin(), data.end(), comparator);
  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
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
}
