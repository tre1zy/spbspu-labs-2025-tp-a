#include "output_formatters.hpp"
#include "format_guard.hpp"

#include <iomanip>

namespace fedorov {
namespace output {
std::ostream &operator<<(std::ostream &out, const Complex &src)
{
  FormatGuard guard(out);
  return out << "#c(" << std::fixed << std::setprecision(1) << src.ref.real() << " "
             << src.ref.imag() << ")";
}

std::ostream &operator<<(std::ostream &out, const Pair &src)
{
  return out << "(:N " << src.ref.first << ":D " << src.ref.second << ":)";
}

std::ostream &operator<<(std::ostream &out, const String &src)
{
  return out << "\"" << src.ref << "\"";
}
} // namespace output
} // namespace fedorov