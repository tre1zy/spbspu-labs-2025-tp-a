#include "data_struct.hpp"
#include <cmath>
#include <complex>
#include <iostream>
#include <utility>

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

std::istream & evstyunichev::operator>>(std::istream &in, KeyIO &key)
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

std::istream & evstyunichev::operator>>(std::istream &in, DataStruct &data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{ '(' };
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
