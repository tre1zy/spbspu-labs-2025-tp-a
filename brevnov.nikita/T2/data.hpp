#ifndef DATA_HPP
#define DATA_HPP
#include <complex>
#include <iostream>
#include <string>
namespace brevnov 
{
  struct DataStruct
  {
    unsigned long long key1;
    std::complex<double> key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct UnLongLongIO
  {
    unsigned long & ref;
  };

  struct ComplexIO
  {
    std::complex< double >& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char > & s);
    ~StreamGuard();
  private:
    std::basic_ios< char > & s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}
#endif
