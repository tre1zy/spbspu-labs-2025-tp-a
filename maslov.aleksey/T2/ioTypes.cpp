#include "ioTypes.hpp"
#include <iomanip>
#include "streamGuard.hpp"

std::ostream & maslov::operator<<(std::ostream & out, const DataStruct & data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  maslov::StreamGuard guard(out);
  out << "(";
  out << ":key1 " << std::fixed << std::setprecision(6) << data.key1;
  out << ":key2 '" << data.key2;
  out << "':key3 \"" << data.key3;
  out << "\":)";
  return out;
}
