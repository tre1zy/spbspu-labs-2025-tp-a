#include "types.hpp"
#include <delimiter.hpp>

using check = shapkov::delimiterIO;
std::istream& shapkov::operator>>(std::istream& in, doubleScientificIO& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  return in >> rhs.key;
}

std::istream& shapkov::operator>>(std::istream& in, ratioIO& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  long long llTemp = 0;
  unsigned long long ullTemp = 0;
  in >> check{ '(' } >> check{ ':' } >> check{ 'N' };
  in >> llTemp;
  in >> check{ ':' } >> check{ 'D' };
  in >> ullTemp;
  in >> check{ ':' } >> check{ ')' };
  if (in)
  {
    rhs.key = { llTemp, ullTemp };
  }
  return in;
}

std::istream& shapkov::operator>>(std::istream& in, stringIO& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  return std::getline(in >> check{ '"' }, rhs.key, '"');
}
