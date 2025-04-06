#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include "keys.hpp"

int main()
{
  using namespace alymova;
  StreamGuard os_sg(std::cout);
  StreamGuard is_sg(std::cin);

  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    if (!std::cin)
    {
      std::cin.clear(std::cin.rdstate() ^ std::ios_base::failbit);
    }
    std::copy(
      std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(data)
    );
  }
  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
}
