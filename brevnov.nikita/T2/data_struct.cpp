#include "data_struct.hpp"

brevnov::StreamGuard::StreamGuard(std::basic_ios< char > & s):
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}

brevnov::StreamGuard::~StreamGuard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}

std::istream& brevnov::operator>>(std::istream& input, DelimeterIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  char c = '0';
  input >> c;
  if (input && (std::tolower(c) != dest.exp))
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& brevnov::operator>>(std::istream& input, DelimetersIO&& dest)
{
  for (char c: dest.exp)
  {
    input >> DelimeterIO{ c };
  }
  return input;
}

std::istream& brevnov::operator>>(std::istream& input, UnLongLongIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  std::string num;
  std::getline(input >> DelimeterIO{ '0' }, num, ':');
  if (num.empty())
  {
    input.setstate(std::ios::failbit);
    return input;
  }
  try
  {
    dest.ref = std::stoull(num, nullptr, 8);
  }
  catch (const std::exception&)
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}


