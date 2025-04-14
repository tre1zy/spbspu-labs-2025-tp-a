#include "datastruct.h"
#include <algorithm>
#include <vector>

int main()
{
  std::vector<asafov::DataStruct> data;
  asafov::DataStruct temp;

  while (!std::cin.eof())
  {
    try
    {
      std::cin >> temp;
      data.push_back(temp);
    }
    catch (const std::logic_error&)
    {

    }
  }
  std::sort(data.begin(), data.end(), asafov::cmpDataStruct);
  if (data.size() == 0) return 0;
  for (const auto& ds : data) std::cout << ds << "\n";

  return 0;
}
