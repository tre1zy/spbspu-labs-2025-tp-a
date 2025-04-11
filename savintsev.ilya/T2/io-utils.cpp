#include "io-utils.hpp"
#include <cmath>

std::istream & savintsev::operator>>(std::istream & in, DelimiterIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '0';
  in >> c;

  if (in && (c != dest.exp_))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream & savintsev::operator>>(std::istream & in, DoubleIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  //std::string number;
  //std::getline(in, number, ':');

  int before = 0, power = 0;
  size_t after = 0;
  char c = '0';

  in >> before;
  in >> c;
  if (c != '.')
  {
    in.setstate(std::ios::failbit);
  }
  in >> after;
  in >> c;
  if (c != 'e' && c != 'E')
  {
    in.setstate(std::ios::failbit);
  }
  in >> power;

  if (!in)
  {
    return in;
  }

  double fractional = after / std::pow(10.0, std::to_string(after).length());
  double result = (before < 0 ? before - fractional : before + fractional) * std::pow(10.0, power);

  dest.ref_ = result;

  return in;
}

std::istream & savintsev::operator>>(std::istream & in, StringIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{'"'}, dest.ref_, '"');
}

std::istream & savintsev::operator>>(std::istream & in, UllIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::ios::fmtflags f = in.flags();
  in >> std::oct >> dest.ref_;
  in.flags(f);

  return in;
}

std::istream & savintsev::operator>>(std::istream & in, LabelIO & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string data = "";
  if (!std::getline(in, data, ' '))
  {
    in.setstate(std::ios::failbit);
  }
  dest.exp_ = data;
  return in;
}
