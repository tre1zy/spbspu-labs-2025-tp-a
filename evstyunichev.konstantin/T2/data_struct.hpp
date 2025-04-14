#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <complex>

namespace evstyunichev
{
  struct DataStruct
  {
    unsigned long long key1;
    std::complex< double > key2;
    std::string key3;
  };
  std::istream & operator>>(std::istream &in, DataStruct &data);
  std::ostream & operator<<(std::ostream &out, const DataStruct &data);

  bool comparator(const DataStruct &, const DataStruct &);

  struct DelimeterIO
  {
    char exp;
  };

  struct UllIO
  {
    unsigned long long &ref;
  };

  struct CmpIO
  {
    std::complex< double > &cmp;
  };

  struct StringIO
  {
    std::string &ref;
  };
}

#endif