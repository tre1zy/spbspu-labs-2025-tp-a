#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <complex>
#include <string>
#include <utility>

namespace khokhryakova
{
  struct DataStruct
  {
    std::complex<double> key1;
    std::pair<long long, unsigned long long> key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct ComplexIO
  {
    std::complex<double>& ref;
  };

  struct RationalIO
  {
    std::pair<long long, unsigned long long>& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, ComplexIO&& dest);
  std::istream& operator>>(std::istream& in, RationalIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& src);

  bool operator<(const DataStruct& a, const DataStruct& b);
}

#endif
