#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include "structs.hpp"
#include "stream-guard.hpp"
int main()
{
  using namespace alymova;
  StreamGuard os_sg(std::cout);
  StreamGuard is_sg(std::cin);

  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
    }
    std::copy(
      std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(data)
    );
  }
  std::sort(data.begin(), data.end());
  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
}
