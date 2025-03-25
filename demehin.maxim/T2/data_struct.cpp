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
  if (in && (std::tolower(c) != dest.exp))
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

std::istream& demehin::ioStructs::operator>>(std::istream& in, KeyNumIO& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string label = "";
  in >> label;
  if (label.substr(0, 3) != "key")
  {
    in.setstate(std::ios::failbit);
  }
  dest.ref = label.back();
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
    using keyNum = KeyNumIO;
    using dbl = DoubleIO;
    using ll = LlIO;
    using str = StringIO;
    in >> sep{ '(' } >> sep{ ':' };
    for (size_t i = 0; i < 3; i++)
    {
      keyNum key_num{ 0 };
      in >> key_num;
      if (key_num.ref == '1')
      {
        in >> ll{ input.key1 };
      }
      else if (key_num.ref == '2')
      {
        in >> dbl{ input.key2 };
      }
      else if (key_num.ref == '3')
      {
        in >> str{ input.key3 };
      }
      in >> sep{ ':' };
    }
    in >> sep { ')' };
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
