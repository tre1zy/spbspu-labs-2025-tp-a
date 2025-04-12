#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_OUTPUT_UTILS_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_OUTPUT_UTILS_HPP

#include <iosfwd>
#include <string>
#include "data-struct.hpp"

namespace kizhin {
  namespace detail {
    namespace output {

      struct Label;
      struct String;
      struct Double;
      struct Rational;

      std::ostream& operator<<(std::ostream&, const Label&);
      std::ostream& operator<<(std::ostream&, const String&);
      std::ostream& operator<<(std::ostream&, const Double&);
      std::ostream& operator<<(std::ostream&, const Rational&);
    }
  }
  std::ostream& operator<<(std::ostream&, const DataStruct&);
}

struct kizhin::detail::output::Label
{
  const std::string& val;
};

struct kizhin::detail::output::String
{
  const std::string& val;
};

struct kizhin::detail::output::Double
{
  const double& val;
};

struct kizhin::detail::output::Rational
{
  const std::pair< long long, unsigned long long >& val;
};

#endif

