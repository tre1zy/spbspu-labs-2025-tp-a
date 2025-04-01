#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

struct DataStruct {
  long long key1;
  std::pair< long long, unsigned long long > key2;
  std::string key3;
};

std::istream& operator>>(std::istream& input, std::pair< long long, unsigned long long >& value);

std::ostream& operator<<(std::ostream& output, const std::pair< long long, unsigned long long >& value);

std::istream& operator>>(std::istream& input, DataStruct& value);

std::ostream& operator<<(std::ostream& output, const DataStruct& value);

int main()
{
  std::vector< DataStruct > data;
  std::copy(std::istream_iterator< DataStruct >(std::cin),
            std::istream_iterator< DataStruct >(),
            std::back_inserter(data));

  return 0;
}
