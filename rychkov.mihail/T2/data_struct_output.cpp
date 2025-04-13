#include "data_struct.hpp"

#include <ostream>
#include <cmath>
#include "ios_guard.hpp"
#include "skip_any_of.hpp"

template<>
std::ostream& rychkov::iofmt::operator<<< rychkov::iofmt::science_literal::value_type,
      rychkov::iofmt::science_literal::id >(std::ostream& out, const science_literal&& link)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  IosGuard guard(out);
  int power = (link.link == 0) ? 0 : 1 + static_cast< int >(std::floor(std::log10(std::fabs(link.link))));
  return out << std::fixed << link.link * std::pow(10 , -power) << 'e' << std::showpos << power;
}
template<>
std::ostream& rychkov::iofmt::operator<<< rychkov::iofmt::ull_literal::value_type,
      rychkov::iofmt::ull_literal::id >(std::ostream& out, const ull_literal&& link)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << link.link << "LL";
}
template<>
std::ostream& rychkov::iofmt::operator<<< rychkov::iofmt::string_literal::value_type,
      rychkov::iofmt::string_literal::id >(std::ostream& out, const string_literal&& link)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << '"' << link.link << '"';
}
std::ostream& rychkov::operator<<(std::ostream& out, const DataStruct& link)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << "(:key1 " << iofmt::science_literal(link.key1) << ":key2 " << iofmt::ull_literal(link.key2);
  return out << ":key3 " << iofmt::string_literal(link.key3) << ":)";
}
