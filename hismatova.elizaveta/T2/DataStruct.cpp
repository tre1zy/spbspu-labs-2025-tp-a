#include "DataStruct.hpp"
#include <iomanip>
#include <vector>
#include <ios>

class StreamGuard
{
public:
  explicit StreamGuard(std::ostream& stream):
    stream_(stream),
    flags_(stream.flags())
  {}
  ~StreamGuard()
  {
    stream_.flags(flags_);
  }
private:
  std::ostream& stream_;
  std::ios::fmtflags flags_;
};

std::istream& hismatova::operator>>(std::istream& in, hismatova::CharIO&& data)
{
  std::istream::sentry sen(in);
  if (!sen)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != data.ch))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& hismatova::operator>>(std::istream& in, hismatova::ULLIO&& data)
{
  std::istream::sentry sen(in);
  if (!sen)
  {
    return in;
  }
  char first, second, third;
  in >> first;
  if (!in || (first != 'u' && first != 'U'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> second;
  if (!in || (second != 'l' && second != 'L'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> third;
  if (!in || (third != 'l' && third != 'L'))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& hismatova::operator>>(std::istream& in, hismatova::NumberIO&& data)
{
  std::istream::sentry sen(in);
  if (!sen)
  {
    return in;
  }
  in >> data.ref >> hismatova::ULLIO{};
  return in;
}

std::istream& hismatova::operator>>(std::istream& in, hismatova::ComplexIO&& data)
{
  std::istream::sentry sen(in);
  if (!sen)
  {
    return in;
  }
  double real;
  double img;
  using io = hismatova::CharIO;
  in >> io{'#'} >> io{'c'} >> io{'('} >> real >> img >> io{')'};
  if (in)
  {
    data.ref = std::complex< double >(real, img);
  }
  return in;
}

std::istream& hismatova::operator>>(std::istream& in, hismatova::StringIO&& data)
{
  std::istream::sentry sen(in);
  if (!sen)
  {
    return in;
  }
  return std::getline(in >> hismatova::CharIO{'"'}, data.ref, '"');
}

std::istream& hismatova::operator>>(std::istream& in, hismatova::DataStruct& data)
{
  std::istream::sentry sen(in);
  if (!sen)
  {
    return in;
  }
  hismatova::DataStruct inp;
  {
    in >> hismatova::CharIO{'('};
    std::vector< std::string > keys(3);
    for (int i = 0; i < 3; ++i)
    {
      in >> hismatova::CharIO{':'} >> keys[i];
      if (keys[i] == "key1")
      {
        in >> hismatova::NumberIO{inp.key1};
      }
      else if (keys[i] == "key2")
      {
        in >> hismatova::ComplexIO{inp.key2};
      }
      else if (keys[i] == "key3")
      {
        in >> hismatova::StringIO{inp.key3};
      }
      else
      {
        in.setstate(std::ios::failbit);
      }
    }
    in >> hismatova::CharIO{':'} >> hismatova::CharIO{')'};
  }
  if (in)
  {
    data = inp;
  }
  return in;
}

std::ostream& hismatova::operator<<(std::ostream& out, const hismatova::DataStruct& data)
{
  StreamGuard guard(out);
  const char* p1 = "(:key1 ";
  const char* p2 = "ull:key2 #c ";
  const char* suf = "):key3 \"";
  const char* end = "\":)";
  auto fix1 = std::fixed << std::setprecision(1);
  out << p1 << data.key1 << p2 << fix1 << data.key2.real() << " " << data.key2.imag() << suf << data.key3 << end;
  return out;
}
