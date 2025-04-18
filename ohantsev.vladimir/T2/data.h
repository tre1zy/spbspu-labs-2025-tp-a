#ifndef DATA_H
#define DATA_H
#include <iosfwd>
#include <string>
#include <array>

namespace ohantsev
{
  struct Data
  {
    double key1;
    unsigned long long key2;
    std::string key3;
    bool operator<(const ohantsev::Data& rhs) const;
    static constexpr int FIELDS_COUNT = 3;
  };

  enum class KeyID
  {
    DBL = 1,
    ULL = 2,
    STR = 3
  };

  struct DelimiterIO
  {
    const char& exp;
  };

  struct MultDelimiterIO
  {
    const std::string& exp;
  };

  struct DoubleI
  {
    double& ref;
  };

  struct DoubleO
  {
    const double& ref;
  };

  struct UllI
  {
    unsigned long long& ref;
  };

  struct UllO
  {
    const unsigned long long& ref;
  };

  struct StringI
  {
    std::string& ref;
  };

  struct StringO
  {
    const std::string& ref;
  };

  struct LabelIO
  {
    KeyID& ID;
    std::array< bool, Data::FIELDS_COUNT >& filled;
  };

  struct TypenameIO
  {
    std::string exp;
  };

  struct KeyIO
  {
    Data& data;
    std::array< bool, Data::FIELDS_COUNT >& filled;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, MultDelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleI&& dest);
  std::istream& operator>>(std::istream& in, UllI&& dest);
  std::istream& operator>>(std::istream& in, StringI&& dest);
  std::istream& operator>>(std::istream& in, LabelIO&& dest);
  std::istream& operator>>(std::istream& in, TypenameIO&& dest);
  std::istream& operator>>(std::istream& in, Data& dest);
  std::istream& operator>>(std::istream& in, KeyIO&& dest);
  std::ostream& operator<<(std::ostream& out, DoubleO&& dest);
  std::ostream& operator<<(std::ostream& out, UllO&& dest);
  std::ostream& operator<<(std::ostream& out, StringO&& dest);
  std::ostream& operator<<(std::ostream& out, const Data& dest);
}
#endif
