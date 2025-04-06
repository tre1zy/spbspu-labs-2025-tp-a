#include <sstream>
#include <vector>
#include "data-struct.hpp"

int main()
{
  using savintsev::DataStruct;

  DataStruct data;
  std::istringstream iss("{\"key1\": 1.0d:\"key2\":354:\"key3\":lox}");
  iss >> data;
}
