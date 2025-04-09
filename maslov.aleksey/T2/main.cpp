#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include "streamGuard.hpp"

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
std::ostream & operator<<(std::ostream & out, const DataStruct & data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  maslov::StreamGuard guard(out);
  out << "(";
  out << ":key1 " << std::fixed << std::setprecision(6) << data.key1;
  out << ":key2 '" << data.key2;
  out << "':key3 \"" << data.key3;
  out << "\":)";
  return out;
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
