#include "delimeters.hpp"
#include <algorithm>
#include <istream>
#include <iterator>
#include <ostream>
#include <string>

std::istream& kiselev::operator>>(std::istream& input, DelimeterIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  char c;
  input >> c;
  if (input && (c != dest.exp))
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& kiselev::operator>>(std::istream& input, DelimetersIO&& dest)
{
  std::string str;
  std::copy_n(std::istream_iterator< char >(input), dest.exp.size(), str.begin());
  if (str != dest.exp)
  {
    input.setstate(std::ios::failbit);
  }
  return input;
}
