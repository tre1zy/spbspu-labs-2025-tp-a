#ifndef OUTPUT_FORMATTERS_HPP
#define OUTPUT_FORMATTERS_HPP

#include <complex>
#include <ostream>
#include <string>
#include <utility>

namespace fedorov {
namespace output {
struct Complex {
  const std::complex<double>& ref;
};
struct Pair {
  const std::pair<long long, unsigned long long>& ref;
};
struct String {
  const std::string& ref;
};

std::ostream& operator<<(std::ostream& out, const Complex& src);
std::ostream& operator<<(std::ostream& out, const Pair& src);
std::ostream& operator<<(std::ostream& out, const String& src);
} // namespace output
} // namespace fedorov

#endif
