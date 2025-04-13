#include "input_parsers.hpp"

#include <cctype>

namespace fedorov {
namespace input {
std::istream &operator>>(std::istream &in, Char &&dest)
{
  char c;
  if ( in >> c && c != dest.expected ) in.setstate(std::ios::failbit);

  return in;
}

std::istream &operator>>(std::istream &in, Complex &&dest)
{
  char c;
  if ( in >> c && c == '#' && in.get() == 'c' && in.get() == 'c' ) {
    double real, imag;
    if ( in >> real >> imag && in.get() == ')' ) {
      dest.ref = {real, imag};
    } else {
      in.setstate(std::ios::failbit);
    }
  } else {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream &operator>>(std::istream &in, Pair &&dest)
{
  in >> Char{'('} >> Char{':'} >> Char{'N'} >> dest.ref.first >> Char{':'} >> Char{'D'} >>
      dest.ref.second >> Char{':'} >> Char{')'} >> Char{':'};

  return in;
}

std::istream &operator>>(std::istream &in, String &&dest)
{
  in >> Char{'"'};
  std::getline(in, dest.ref, '"');
  return in >> Char{':'};
}
} // namespace input
} // namespace fedorov
