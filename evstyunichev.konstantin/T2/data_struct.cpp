#include "data_struct.hpp"
#include <cmath>
#include <complex>
#include <ios>
#include <iostream>
#include <utility>
#include "stream_guard.hpp"

double evstyunichev::abscmpl(const std::complex< double > &cmpl)
{
  return std::pow(std::pow(cmpl.real(), 2) + std::pow(cmpl.imag(), 2), 0.5);
}

std::istream & evstyunichev::operator>>(std::istream& in, DelimiterIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp) && (tolower(c) != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & evstyunichev::operator>>(std::istream& in, DelimitersIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  for (char c: dest.exps)
  {
    in >> DelimiterIO{ std::move(c) };
    if (!in)
    {
      return in;
    }
  }
  return in;
}

std::istream & evstyunichev::operator>>(std::istream &in, UllIO &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref;
  if (!in)
  {
    return in;
  }
  in >> DelimitersIO{ "ull" };
  return in;
}

std::istream & evstyunichev::operator>>(std::istream &in, CmplIO &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  if (in >> DelimitersIO{ "#c(" })
  {
    in >> dest.cmpl >> DelimiterIO{ ')' };
  }
  return in;
}

std::istream & evstyunichev::operator>>(std::istream &in, KeyIO &&key)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimitersIO{ ":key" };
  int k = 0;
  in >> k;
  if (k == 1)
  {
    in >> key.data.key1;
  }
  else if (k == 2)
  {
    in >> key.data.key2;
  }
  else if (k == 3)
  {
    in >> key.data.key3;
  }
  return in;
}

std::istream & evstyunichev::operator>>(std::istream &in, StringIO &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{ '"' };
  std::getline(in, dest.ref, '"');
  return in;
}

std::istream & evstyunichev::operator>>(std::istream &in, DataStruct &data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  evstyunichev::StreamGuard guard(in);
  DataStruct temp;
  if (in >> DelimiterIO{ '(' })
  {
    in >> KeyIO{ temp };
    in >> KeyIO{ temp };
    in >> KeyIO{ temp };
    in >> DelimitersIO{ ":)" };
  }
  if (in)
  {
    data = temp;
  }
  return in;
}

std::ostream & evstyunichev::operator<<(std::ostream &out, UllIO &&data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << data.ref << "ull";
}

std::ostream & evstyunichev::operator<<(std::ostream &out, CmplIO &&data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << "#c(" << data.cmpl.real() << ' ' << data.cmpl.imag() << ')';
}

std::ostream & evstyunichev::operator<<(std::ostream &out, StringIO &&data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << '"' << data.ref << '"';
}

std::ostream & evstyunichev::operator<<(std::ostream &out, const DataStruct &data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard guard(out);
  unsigned long long ull = data.key1;
  std::complex< double > cmpl = data.key2;
  std::string string = data.key3;
  out << "(:key1 " << UllIO{ ull };
  out << ":key2 " << CmplIO{ cmpl };
  out << ":key3 " << StringIO{ string } << ":)";
  return out;
}

bool evstyunichev::comparator(const DataStruct &a, const DataStruct &b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  if (a.key2 != b.key2)
  {
    return abscmpl(a.key2) < abscmpl(b.key2);
  }
  return a.key3.size() < b.key3.size();

}
