#include "dataStruct.hpp"
#include <vector>
#include <iterator>
#include <algorithm>

int main()
{
  std::vector< DataStruct > data;
  std::istream_iterator< DataStruct > inputBegin(std::cin);
  std::istream_iterator< DataStruct > inputEnd;
  while (inputBegin != inputEnd)
  {
    data.push_back(*inputBegin);
    if (!std::cin)
    {
      std::cin.clear();
      std::string line;
      std::getline(std::cin, line);
    }
    ++inputBegin;
  }
  std::sort(data.begin(), data.end());
  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
  return 0;
}
