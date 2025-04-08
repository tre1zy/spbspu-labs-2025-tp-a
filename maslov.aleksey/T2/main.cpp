#include <string>
#include <vector>
#include <iostream>
#include <iterator>

struct DataStruct
 {
  double key1;
  char key2;
  std::string key3;
 };

int main()
{
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
    }
  }
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
}
