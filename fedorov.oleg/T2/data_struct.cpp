#include "data_struct.hpp"
#include "input_parsers.hpp"
#include "output_formatters.hpp"
#include "format_guard.hpp"

#include <cmath>

namespace fedorov {
bool operator<(const DataStruct &a, const DataStruct &b)
{
  const double modA = std::abs(a.key1);
  const double modB = std::abs(b.key1);

  if ( std::abs(modA - modB) > 1e-6 ) return modA < modB;
  if ( a.key2 != b.key2 ) return a.key2 < b.key2;
  return a.key3.length() < b.key3.length();
}

std::istream &operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if ( !sentry ) return in;

  dest = DataStruct{};
  bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
  std::string line;

  std::getline(in >> std::ws, line);
  std::istringstream iss(line);

  iss >> input::Char{'('} >> input::Char{':'};

  std::string key;
  while ( iss >> input::Char{':'} >> key ) {
    if ( key == "key1" ) {
      iss >> input::Complex{dest.key1};
      hasKey1 = true;
    } else if ( key == "key2" ) {
      iss >> input::Pair{dest.key2};
      hasKey2 = true;
    } else if ( key == "key3" ) {
      iss >> input::String{dest.key3};
      hasKey3 = true;
    } else {
      iss.setstate(std::ios::failbit);
      break;
    }

    if ( iss.peek() == ')' ) break;
  }

  iss >> input::Char{')'};

  if ( !(hasKey1 && hasKey2 && hasKey3) ) {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream &operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if ( !sentry ) return out;
  FormatGuard guard(out);

  out << "(:key1 " << output::Complex{src.key1} << ":"
      << "key2 " << output::Pair{src.key2} << ":"
      << "key3 " << output::String{src.key3} << ":)";

  return out;
}
} // namespace fedorov
