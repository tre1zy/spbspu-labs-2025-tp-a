#ifndef INPUT_PARSERS_HPP
#define INPUT_PARSERS_HPP

#include <complex>
#include <istream>
#include <string>
#include <utility>

namespace fedorov {
namespace input {
struct Char {
  char expected;
};
struct Complex {
  std::complex<double>& ref;
};
struct Pair {
  std::pair<long long, unsigned long long>& ref;
};
struct String {
  std::string& ref;
};

std::istream& operator>>(std::istream& in, Char&& dest);
std::istream& operator>>(std::istream& in, Complex&& dest);
std::istream& operator>>(std::istream& in, Pair&& dest);
std::istream& operator>>(std::istream& in, String&& dest);
} // namespace input
} // namespace fedorov

#endif