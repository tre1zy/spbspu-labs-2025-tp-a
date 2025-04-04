#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <string>

namespace abramov
{
  struct DataStruct
  {
    double key1;
    unsigned long long int key2;
    std::string key3;
  };

  std::ostream &operator<<(std::ostream &out, const DataStruct &data);
}
#endif
