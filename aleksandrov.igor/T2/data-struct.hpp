#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iostream>
#include <iomanip>
#include <utility>
#include <string>

namespace aleksandrov
{
  struct DataStruct
  {
    double key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
  };

  class iofmtguard
  {
  public:
    explicit iofmtguard(std::basic_ios< char >&);
    ~iofmtguard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  bool operator<(const DataStruct&, const DataStruct&);
}

#endif

