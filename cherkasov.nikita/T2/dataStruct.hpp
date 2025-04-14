#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <string>
#include <iostream>
#include <complex>
#include <utility>

namespace cherkasov
{
  struct DataStruct
  {
    std::complex< double > key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;

    bool operator<(const DataStruct & other) const;
  };

  struct ComplexIO
  {
    std::complex< double > & c;
  };

  struct RationalIO
  {
    std::pair< long long, unsigned long long > & rat;
  };

  struct StringIO
  {
    std::string & s;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct LabelIO
  {
    std::string exp;
  };

  std::istream & operator>>(std::istream & in, DelimiterIO&&);
  std::istream & operator>>(std::istream & in, LabelIO&&);
  std::istream & operator>>(std::istream & in, ComplexIO&&);
  std::istream & operator>>(std::istream & in, RationalIO&&);
  std::istream & operator>>(std::istream & in, StringIO&&);
  std::istream & operator>>(std::istream & in, DataStruct&);
  std::ostream & operator<<(std::ostream & out, const DataStruct&);
}

#endif
