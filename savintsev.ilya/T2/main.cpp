#include <sstream>
#include <vector>
#include "data-struct.hpp"

int main()
{
  using savintsev::DataStruct;

  DataStruct data;
  std::istringstream iss("(:key1 1.0e+2:key2 077:key3 \"value\":)");
  iss >> data;

  if (!iss)
  {
    std::cout << "grustno..\n";
    return 0;
  }
  std::cout << "norm\n";
}
