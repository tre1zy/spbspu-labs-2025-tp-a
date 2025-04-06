#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_IO_UTILS_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_IO_UTILS_HPP

#include <iosfwd>
#include <string>
#include "data-struct.hpp"

namespace kizhin {
  namespace detail {
    template < typename T >
    struct ReferenceStorage;

    using DoubleIO = ReferenceStorage< double >;
    using NominatorIo = ReferenceStorage< long long >;
    using DenominatorIO = ReferenceStorage< unsigned long long >;
    using RationalIO = ReferenceStorage< std::pair< long long, unsigned long long > >;
    using StringIO = ReferenceStorage< std::string >;

    struct DelimiterIO;
    struct MultiDelimiterIO;
    struct LabelIO;

    std::istream& operator>>(std::istream&, DelimiterIO&&);
    std::istream& operator>>(std::istream&, MultiDelimiterIO&&);
    std::istream& operator>>(std::istream&, LabelIO&&);
    std::istream& operator>>(std::istream&, DoubleIO&&);
    std::istream& operator>>(std::istream&, NominatorIo&&);
    std::istream& operator>>(std::istream&, DenominatorIO&&);
    std::istream& operator>>(std::istream&, RationalIO&&);
    std::istream& operator>>(std::istream&, StringIO&&);
  }
  std::istream& operator>>(std::istream&, DataStruct&);
  std::ostream& operator<<(std::ostream&, const DataStruct&);
}

template < typename T >
struct kizhin::detail::ReferenceStorage
{
  T& ref;
};

struct kizhin::detail::DelimiterIO
{
  char exp;
};

struct kizhin::detail::MultiDelimiterIO
{
  std::string exp;
};

struct kizhin::detail::LabelIO
{
  std::string exp;
};

#endif

