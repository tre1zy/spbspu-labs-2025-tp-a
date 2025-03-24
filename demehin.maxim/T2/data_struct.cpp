#include "data_struct.hpp"
#include <cctype>
#include <cmath>
#include <sstream>
#include "scope_guard.hpp"

namespace
{
  using namespace demehin::ioStructs;
}

std::istream& demehin::ioStructs::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& demehin::ioStructs::operator>>(std::istream& in, DoubleIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> dest.ref;
}

std::istream& demehin::ioStructs::operator>>(std::istream& in, LlIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return in >> dest.ref >> DelimiterIO{ 'l' } >> DelimiterIO{ 'l' };
}

std::istream& demehin::ioStructs::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& demehin::ioStructs::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string label = "";
  in >> label;
  if (label != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& demehin::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    using sep = DelimiterIO;
    using label = LabelIO;
    using dbl = DoubleIO;
    using ll = LlIO;
    using str = StringIO;
    in >> sep{ '(' } >> sep{ ':' };
    in >> label{ "key1" } >> dbl{ input.key1 };
    in >> sep{ ':' };
    in >> label{ "key2" } >> ll{ input.key2 };
    in >> sep{ ':' };
    in >> label{ "key3" } >> str{ input.key3 };
    in >> sep{ ':' } >> sep{ ')' };
  }
  if (in)
  {
    dest = input;
  }
  return in;
}

std::ostream& demehin::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  iofmtguard fmtguard(out);

  out << std::scientific;
  out << "(:key1 " << src.key1;
  out << ":key2 " << src.key2 << "ll";
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}
