#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <iosfwd>

namespace rychkov
{
  struct DataStruct
  {
    double key1;
    long long key2;
    std::string key3;
  };
  std::istream& operator>>(std::istream& in, DataStruct& link);

  struct nth_ds_field
  {
    size_t field_number;
    DataStruct& link;
  };
  std::istream& operator>>(std::istream& in, nth_ds_field link);
}

#endif
