#include "datastruct.hpp"
#include <iomanip>
#include "stream_guard.hpp"

std::ostream &abramov::operator<<(std::ostream &out, const DataStruct &data)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  StreamGuard guard(out);
  out << "(";
  out << ":key1 " << std::fixed << std::setprecision(1) << data.key1 << "d";
  out << ":key2 " << data.key2 << "ull";
  out << ":key3 " << "\"" << data.key3 << "\"";
  out << ":)";
  return out;
}
