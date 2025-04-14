#ifndef DATA_HPP
#define DATA_HPP

#include <string>

namespace dribas
{
  struct Data
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  struct UllI
  {
    unsigned long long& ref;
  };

  struct DoubleI
  {
    double& ref;
  };

  struct StringI
  {
    std::string& ref;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct UllO
  {
    const unsigned long long& ref;
  };

  struct DoubleO
  {
    const double& ref;
  };

  struct StringO
  {
    const std::string& ref;
  };

  std::istream& operator>>(std::istream&, Data&);
  std::istream& operator>>(std::istream&, DelimiterIO&&);
  std::istream& operator>>(std::istream&, DoubleI&&);
  std::istream& operator>>(std::istream&, UllI&&);
  std::istream& operator>>(std::istream&, StringI&&);

  std::ostream& operator<<(std::ostream&, const DoubleO&&);
  std::ostream& operator<<(std::ostream&, const UllO&&);
  std::ostream& operator<<(std::ostream&, const StringO&&);
  std::ostream& operator<<(std::ostream&, const Data&);

  bool compare(const Data&, const Data&);
}


#endif
