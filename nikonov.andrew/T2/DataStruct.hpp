#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <string>
#include <istream>
namespace nikonov
{
  struct DataStruct
  {
    double key1_;
    unsigned long long key2_;
    std::string key3_;
  };

  namespace detail
  {
    struct DelimiterIO
    {
      char exp_;
    };
    struct DoubleIO
    {
      double& ref_;
    };
    struct UnsignedLongLong
    {
      unsigned long long& ref_;
    };
    struct StringIO
    {
      std::string& ref_;
    };
    struct LabelIO
    {
      std::string exp_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, DoubleIO&& dest);
    std::istream& operator>>(std::istream& in, UnsignedLongLong&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);

    std::ostream& operator<<(std::ostream& out, DoubleIO&& dest);
    std::ostream& operator<<(std::ostream& out, UnsignedLongLong&& dest);
    std::ostream& operator<<(std::ostream& out, StringIO&& dest);
  }
  std::ostream& operator>>(std::ostream& out, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}
#endif
