#include "DataStruct.hpp"
#include <iomanip>
#include "StreamGuard.hpp"
#include "IO-utilities.hpp"
namespace nikonov
{
  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    DataStruct input;
    {
      using sep = detail::DelimiterIO;
      using label = detail::LabelIO;
      using dbl = detail::DoubleIO;
      using ull = detail::UnsignedLongLong;
      using str = detail::StringIO;
      in >> sep{ '(' };
      in >> label{ ":key1" } >> dbl{ input.key1_ };
      in >> label{ ":key2" } >> ull{ input.key2_ };
      in >> label{ ":key3" } >> str{ input.key3_ };
      in >> sep{ ')' };
    }
    if (in)
    {
      dest = input;
    }
    return in;
  }
  std::ostream& operator<<(std::ostream& out, const DataStruct& toOut)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard outGuard(out);
    out << "(";
    out << ":key1 " << std::fixed << std::setprecision(1) << detail::DoubleIO{ const_cast< double & >(toOut.key1_) };
    out << " :key2:" << detail::UnsignedLongLong{ const_cast< unsigned long long& >(toOut.key2_) };
    out << " :key3:" << detail::StringIO{ const_cast< std::string & >(toOut.key3_) };
    out << ")";
    return out;
  }
}
