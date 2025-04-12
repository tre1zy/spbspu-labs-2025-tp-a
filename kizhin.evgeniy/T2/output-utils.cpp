#include "output-utils.hpp"
#include <iomanip>
#include <iostream>
#include "stream-guard.hpp"

std::ostream& kizhin::detail::output::operator<<(std::ostream& out, const Label& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  return out << ':' << data.val << ' ';
}

std::ostream& kizhin::detail::output::operator<<(std::ostream& out, const Rational& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  out << "(";
  out << ":N " << data.val.first;
  out << ":D " << data.val.second;
  return out << ":)";
}

std::ostream& kizhin::detail::output::operator<<(std::ostream& out, const Double& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  return out << std::fixed << std::setprecision(1) << data.val << 'd';
}

std::ostream& kizhin::detail::output::operator<<(std::ostream& out, const String& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  return out << '"' << data.val << '"';
}

std::ostream& kizhin::operator<<(std::ostream& out, const DataStruct& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  StreamGuard guard(out);
  using namespace detail::output;
  out << '(';
  out << Label{ "key1" } << Double{ data.key1 };
  out << Label{ "key2" } << Rational{ data.key2 };
  out << Label{ "key3" } << String{ data.key3 };
  return out << ":)";
}

