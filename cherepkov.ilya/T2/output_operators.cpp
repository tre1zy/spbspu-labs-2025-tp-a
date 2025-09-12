#include "output_operators.hpp"
#include "stream_guard.hpp"

std::ostream& cherepkov::operator<<(std::ostream& out, const UllLitOutput& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry) return out;
  StreamGuard guard(out);
  return out << dest.ref << "ull";
}

std::ostream& cherepkov::operator<<(std::ostream& out, const UllBinOutput& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry) return out;
  StreamGuard guard(out);
  
  if (dest.ref == 0) 
  {
    return out << "0";
  }
  std::string bin;
  bin.reserve(64);
  unsigned long long val = dest.ref;
  
  while (val > 0)
  {
    bin.push_back('0' + (val % 2));
    val /= 2;
  }
  std::reverse(bin.begin(), bin.end());
  
  return out << bin;
}
