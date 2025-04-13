#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <ios>
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

  struct DelimeterIO
  {
    char exp;
  };

  struct DoubleIO
  {
    double& ref;
  };

  struct RationalIO
  {
    std::pair< long long, unsigned long long >& ref;
  };

  struct LongLongIO
  {
    long long& ref;
  };

  struct UnsignedLongLongIO
  {
    unsigned long long& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct LabelIO
  {
    std::string exp;
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

  std::istream& operator>>(std::istream&, DelimeterIO&&);
  std::istream& operator>>(std::istream&, DoubleIO&&);
  std::istream& operator>>(std::istream&, RationalIO&&);
  std::istream& operator>>(std::istream&, LongLongIO&&);
  std::istream& operator>>(std::istream&, UnsignedLongLongIO&&);
  std::istream& operator>>(std::istream&, LabelIO&);
  std::istream& operator>>(std::istream&, DataStruct&);

  std::ostream& operator<<(std::ostream&, const DoubleIO&);
  std::ostream& operator<<(std::ostream&, const RationalIO&);
  std::ostream& operator<<(std::ostream&, const LabelIO&);
  std::ostream& operator<<(std::ostream&, const StringIO&);
  std::ostream& operator<<(std::ostream&, const DataStruct&);
}

#endif

