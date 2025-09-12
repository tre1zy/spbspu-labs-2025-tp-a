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

  if (dest.ref == 1)
  {
    return out << "01";
  }

  std::string bin;
  unsigned long long val = dest.ref;

  while (val > 0)
  {
    bin.insert(0, std::to_string(val % 2));
    val /= 2;
  }

  return out << "0b" << bin;
}
