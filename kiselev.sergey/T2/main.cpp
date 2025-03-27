#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include "data.hpp"
int main()
{
  std::vector< kiselev::DataStruct > data;
  std::istringstream iss("(:key1 076:key2 \'c\':key3 \"Data\":)");
  std::copy
  (
    std::istream_iterator< kiselev::DataStruct >(iss),
    std::istream_iterator< kiselev::DataStruct >(),
    std::back_inserter(data)
  );
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< kiselev::DataStruct >(std::cout, "\n")
  );
  return 0;
}
