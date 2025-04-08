#include "input_struct.hpp"
#include "format_guard.hpp"
#include "data_struct.hpp"

bool filonova::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  if (lhs.key1 == rhs.key1)
  {
    if (std::abs(lhs.key2) == std::abs(rhs.key2))
    {
      return lhs.key3.length() < rhs.key3.length();
    }
    return std::abs(lhs.key2) < std::abs(rhs.key2);
  }
  return lhs.key1 < rhs.key1;
}

std::istream &filonova::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct input;
  using sep = DelimiterValue;
  using hex = HexValue;
  using cmp = ComplexValue;
  using str = StringValue;

  in >> sep{'('};

  for (size_t i = 0; i < 3; i++)
  {
    std::string key;
    in >> sep{':'} >> key;

    if (key == "key1")
    {
      in >> hex{input.key1};
    }
    else if (key == "key2")
    {
      in >> cmp{input.key2};
    }
    else if (key == "key3")
    {
      in >> str{input.key3};
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  in >> sep{':'} >> sep{')'};

  if (in)
  {
    dest = input;
  }
  return in;
}

std::ostream &filonova::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  filonova::FormatGuard guard(out);

  out << "(";
  out << ":key1 0x" << std::hex << std::uppercase << src.key1;
  out << std::fixed << std::setprecision(1);
  out << ":key2 #c(" << src.key2.real() << " " << src.key2.imag() << ")";
  out << ":key3 \"" << src.key3 << "\"";
  out << ":)";

  return out;
}
