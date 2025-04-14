#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <limits>

namespace nspace
{
  struct DataStruct
  {
    char key1;
    std::pair < long long, unsigned long long > key2;
    std::string key3;
  };

  struct DelimetrIO
  {
    char exp;
  };

  struct CharIO
  {
    char& ref;
  };

  struct RationalIO
  {
    std::pair< long long, unsigned long long >& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  class iofmtguard
  {
  public:
    iofmtguard(std::basic_ios< char >& s);
    ~iofmtguard();
  private:
    std::basic_ios< char >& s_;
    char fill_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  std::istream& operator>>(std::istream& in, DelimetrIO&& dest);
  std::istream& operator>>(std::istream& in, CharIO&& dest);
  std::istream& operator>>(std::istream& in, RationalIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}

namespace nspace
{
  std::istream& operator>>(std::istream& in, DelimetrIO&& dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    char c = 0;
    in >> c;
    if (in && c != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, CharIO&& dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    in >> DelimetrIO{ '\'' } >> dest.ref >> DelimetrIO{ '\'' } >> DelimetrIO{ ':' };
    return in;
  }

  std::istream& operator>>(std::istream& in, RationalIO&& dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    in >> DelimetrIO{ '(' } >> DelimetrIO{ ':' } >> DelimetrIO{ 'N' } >> dest.ref.first;
    in >> DelimetrIO{ ':' } >> DelimetrIO{ 'D' } >> dest.ref.second >> DelimetrIO{ ':' };
    in >> DelimetrIO{ ')' } >> DelimetrIO{ ':' };
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    return std::getline(in >> DelimetrIO{ '"' }, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    DataStruct input{};
    {
      using sep = DelimetrIO;
      using chr = CharIO;
      using rtn = RationalIO;
      using str = StringIO;
      in >> sep{ '(' } >> sep{ ':' };
      for (size_t i = 0; i < 3; i++)
      {
        std::string temp = "";
        in >> temp;
        if (temp == "key1")
        {
          in >> chr{ input.key1 };
        }
        else if (temp == "key2")
        {
          in >> rtn{ input.key2 };
        }
        else if (temp == "key3")
        {
          in >> str{ input.key3 } >> sep{ ':' };
        }
      }
      in >> sep{ ')' };
    }
    if (in)
    {
      dest = input;
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& dest)
  {
    std::ostream::sentry s(out);
    if (!s)
    {
      return out;
    }
    out << "(:" << "key1 " << "'" << dest.key1 << "'" << ":";
    out << "key2 " << "(:" << "N " << dest.key2.first << ":" << "D " << dest.key2.second << ":):";
    out << "key3 " << '"' << dest.key3 << '"' << ":)";
    return out;
  }
  iofmtguard::iofmtguard(std::basic_ios< char >& s) :
    s_(s),
    fill_(s.fill()),
    width_(s.width()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  iofmtguard::~iofmtguard()
  {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }
}

bool compare(const nspace::DataStruct& lhs, const nspace::DataStruct& rhs)
{
  bool compareChr = lhs.key1 < rhs.key1;
  bool compareRtn = lhs.key2 < rhs.key2;
  bool compareStr = lhs.key3.size() < rhs.key3.size();
  return lhs.key1 != rhs.key1 ? compareChr : (lhs.key2 != rhs.key2 ? compareRtn : compareStr);
}

int main()
{
  using nspace::DataStruct;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(std::istream_iterator< DataStruct >(std::cin),
    std::istream_iterator< DataStruct >(), std::back_inserter(data));
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), compare);
  std::copy(std::begin(data), std::end(data), std::ostream_iterator< DataStruct >(std::cout, "\n"));
}
