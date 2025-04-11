#include "datastruct.hpp"
#include <iomanip>
#include "stream_guard.hpp"

std::ostream &abramov::operator<<(std::ostream &out, const DataStruct &data)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  StreamGuard guard(out);
  out << "(";
  out << ":key1 " << std::fixed << std::setprecision(1) << data.key1 << "d";
  out << ":key2 " << data.key2 << "ull";
  out << ":key3 " << "\"" << data.key3 << "\"";
  out << ":)";
  return out;
}

std::istream &abramov::operator>>(std::istream &in, DelimiterIO &&dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.delim))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &abramov::operator>>(std::istream &in, DoubleIO &&dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> dest.ref >> DelimiterIO{ 'd' };
  return in;
}

std::istream &abramov::operator>>(std::istream &in, UllIO &&dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> dest.ref >> DelimiterIO{ 'u' } >> DelimiterIO{ 'l' } >> DelimiterIO{ 'l' };
  return in;
}
std::istream &abramov::operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  return std::getline(in, dest.ref, ' ');
}

std::istream &abramov::operator>>(std::istream &in, KeyIO &&dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  std::string data = "";
  if ((in >> StringIO{ data }) && (data != dest.key))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &abramov::operator>>(std::istream &in, DataStruct &data)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  DataStruct input;
  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
  in >> KeyIO{ "key1" };
  in >> DoubleIO{ data.key1 };
  in >> DelimiterIO{ ':' } >> KeyIO{ "key2" };
  in >> UllIO{ data.key2 } >> DelimiterIO{ ':' } >> KeyIO{ "key3" };
  std::string tmp;
  std::getline(in, tmp, ':');
  tmp = tmp.substr(1, tmp.size() - 2);
  data.key3 = tmp;
  in >> DelimiterIO{ ')' };
  return in;
}

bool abramov::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  if (lhs.key1 == lhs.key1)
  {
    if (lhs.key2 == rhs.key2)
    {
      return lhs.key3.size() < rhs.key3.size();
    }
    else
    {
      return lhs.key2 < rhs.key2;
    }
  }
  else
  {
    return lhs.key1 < rhs.key1;
  }
}
