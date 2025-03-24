#include "data_struct.hpp"
#include <vector>
#include <iterator>
#include <sstream>
#include <iostream>
#include <iomanip>

int main()
{
  std::vector< demehin::DataStruct > data;
  std::istringstream iss("(:key1 5.45e-2:key2 10ll:key3 \"Data\":)");
  std::copy
  (
    std::istream_iterator< demehin::DataStruct >(iss),
    std::istream_iterator< demehin::DataStruct >(),
    std::back_inserter(data)
  );
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< demehin::DataStruct >(std::cout, "\n")
  );
}

