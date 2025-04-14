#include "datastruct.h"
#include <algorithm>
#include <vector>

int main()
{
  std::vector<asafov::DataStruct> data;
  asafov::DataStruct temp;

  try
  {
    while (std::cin >> temp) data.push_back(temp);
  }
  catch(std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  std::sort(data.begin(), data.end(), asafov::cmpDataStruct);
  for (const auto& ds : data) std::cout << ds << "\n";

  return 0;
}
