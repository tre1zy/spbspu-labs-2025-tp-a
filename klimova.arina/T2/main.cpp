#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>
#include "datastruct.hpp"

int main()
{
  using klimova::DataStruct;
  using klimova::compareData;
  using IteratorIn = std::istream_iterator< DataStruct >;
  using IteratorOut = std::ostream_iterator< DataStruct >;
  using Limits = std::numeric_limits<std::streamsize>;
  auto& in = std::cin;
  auto& out = std::cout;

  std::vector< DataStruct > data;
  while (!in.eof())
  {
    if (in.fail())
    {
      in.clear();
      in.ignore(Limits::max(), '\n');
    }
    std::copy(
      IteratorIn(in),
      IteratorIn(),
      std::back_inserter(data)
    );
  }
  std::sort(data.begin(), data.end(), compareData);
  std::copy
  (
    data.begin(),
    data.end(),
    IteratorOut(out, "\n")
  );

return 0;
}
