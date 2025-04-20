#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_INPUT_UTILS_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_INPUT_UTILS_HPP

#include <iosfwd>
#include <string>
#include "data-struct.hpp"

namespace kizhin {
  namespace detail {
    namespace input {

      struct Delimiter;
      struct OneOfDelimiters;
      struct Label;
      struct String;
      struct Double;
      struct Nominator;
      struct Denominator;
      struct Rational;

      std::istream& operator>>(std::istream&, Delimiter&&);
      std::istream& operator>>(std::istream&, OneOfDelimiters&&);
      std::istream& operator>>(std::istream&, Label&&);
      std::istream& operator>>(std::istream&, String&&);
      std::istream& operator>>(std::istream&, Double&&);
      std::istream& operator>>(std::istream&, Nominator&&);
      std::istream& operator>>(std::istream&, Denominator&&);
      std::istream& operator>>(std::istream&, Rational&&);
    }
  }
  std::istream& operator>>(std::istream&, DataStruct&);
}

struct kizhin::detail::input::Delimiter
{
  char val;
};

struct kizhin::detail::input::OneOfDelimiters
{
  const std::string& val;
};

struct kizhin::detail::input::Double
{
  double& val;
};

struct kizhin::detail::input::Label
{
  const std::string& val;
};

struct kizhin::detail::input::String
{
  std::string& val;
};

struct kizhin::detail::input::Nominator
{
  long long& val;
};

struct kizhin::detail::input::Denominator
{
  unsigned long long& val;
};

struct kizhin::detail::input::Rational
{
  DataStruct::Rational& val;
};

#endif

