#include "io-helpers.hpp"
#include "stream-guard.hpp"

std::ostream& belyaev::operator<<(std::ostream& out, const DoubleIO& dbl)
{
  belyaev::StreamGuard streamGuard(out);

  int exp = 0;
  double mantissa = dbl.value;

  if (mantissa == 0.0)
  {
    out << "0.0e+0";
    return out;
  }

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

std::ostream& belyaev::operator<<(std::ostream& out, const PairLLIO& pairLL)
{
  belyaev::StreamGuard streamGuard(out);

  out << "(:N ";
  out << pairLL.value.first;
  out << ":D ";
  out << pairLL.value.second;
  out << ":)";
  return out;
}
