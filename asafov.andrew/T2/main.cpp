#include "datastruct.h"
#include <algorithm>
#include <vector>
#include <iterator>

int main()
{
  std::vector<asafov::DataStruct> data;
  try
  {
    std::copy(
      std::istream_iterator<asafov::DataStruct>(std::cin),
      std::istream_iterator<asafov::DataStruct>(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (data.empty())
    {
      return 0;
    }
    std::sort(data.begin(), data.end(), asafov::cmpDataStruct);
    std::copy(
      std::begin(data),
      std::end(data),
      std::ostream_iterator<asafov::DataStruct>(std::cout, "\n")
    );
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
