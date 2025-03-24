#include "data_struct.hpp"
#include <vector>
#include <iterator>
#include <sstream>
#include <iostream>
#include <iomanip>

int main()
{
  std::vector< demehin::DataStruct > data;
  std::copy
  (
    std::istream_iterator< demehin::DataStruct >(std::cin),
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


