#include <limits>
#include <iterator>
#include <vector>
#include "data-struct.hpp"
#include "input-struct.hpp"
#include "output-struct.hpp"

int main()
{
  using namespace aleksandrov;

  std::vector< DataStruct > data;

  while (!std::cin.eof())
  {
    std::copy
    (
      std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
}

