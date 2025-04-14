#include "datastruct.h"
#include <algorithm>
#include <vector>

/*int main()
{
  //(:key1 0b11:key2 #c(1.0 1.0):key3 "dataa":)
  std::vector<asafov::DataStruct> data;
  asafov::DataStruct temp;

  try
  {
    while (std::cin >> temp) data.push_back(temp);
  }
  catch(std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }
  std::sort(data.begin(), data.end(), asafov::cmpDataStruct);
  for (const auto& ds : data) std::cout << ds << "\n";

  return 0;
}*/

int main()
{
  std::noskipws(std::cin);
  asafov::DataStruct data;
  try
  {
    std::cin >> data;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  std::cout << data << '\n';
  return 0;
}
