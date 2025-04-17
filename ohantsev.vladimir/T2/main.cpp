#include <iostream>
#include <algorithm>
#include <iterator>
#include <limits>
#include <string>
#include <vector>
#include "data.h"

int main()
{
  std::vector< ohantsev::Data > data;
  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator< ohantsev::Data >(std::cin),
      std::istream_iterator< ohantsev::Data >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< ohantsev::Data >(std::cout, "\n")
  );
  return 0;
}
