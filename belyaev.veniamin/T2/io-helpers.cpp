#include "io-helpers.hpp"

std::ostream& belyaev::operator<<(std::ostream& out, const DoubleIO& dbl)
{
  // scopeguard?

  int exp = 0;
  double mantissa = 0.0;

  while (std::abs(mantissa) >= 10.0)
  {
    exp++;
    mantissa /= 10.0;
  }

  while (std::abs(mantissa) < 1.0)
  {
    exp--;
    mantissa *= 10.0;
  }

  out << mantissa;
  
  out << "e";
  if (exp >= 0)
  {
    out << "+";
  }
  else
  {
    out << "-";
  }
  out << exp;

  return out;
}