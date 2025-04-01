
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct DataStruct {
  long long key1;
  std::pair< long long, unsigned long long > key2;
  std::string key3;
};

std::istream& operator>>(std::istream& input, DataStruct& value);
std::ostream& operator<<(std::ostream& output, const DataStruct& value);

int main()
{
  return 0;
}
