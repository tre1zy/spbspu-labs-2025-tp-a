#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

struct DataStruct
{
  double key1;
  char key2;
  std::string key3;
  bool operator<(const DataStruct & rhs) const;
};
bool DataStruct::operator<(const DataStruct & rhs) const
{
  if (key1 != rhs.key1)
  {
    return key1 < rhs.key1;
  }
  else if (key2 != rhs.key2)
  {
    return key2 < rhs.key2;
  }
  else
  {
    return key3.length() < rhs.key3.length();
  }
}
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
  std::sort(data.begin(), data.end());
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
}
