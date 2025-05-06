#include "DataStruct.h"

namespace fedorova
{
  std::istream& operator>>(std::istream& is, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(is);
    if (!sentry)
    {
      return is;
    }

    char c;
    is >> c;
    if (is && (c != dest.exp))
    {
      is.setstate(std::ios::failbit);
    }

    return is;
  }

  std::istream& operator>>(std::istream& is, ULLLiteralIO&& dest)
  {
    std::istream::sentry sentry(is);
    if (!sentry)
    {
      return is;
    }

    is >> dest.ref;

    std::string suffix;
    std::getline(is, suffix, ':');
    is.putback(':');

    if (is && suffix != "ull" && suffix != "ULL")
    {
      is.setstate(std::ios::failbit);
    }
    else
    {
      dest.suffix = suffix;
    }

    return is;
  }

  std::istream& operator>>(std::istream& is, ULLBinaryIO&& dest)
  {
    std::istream::sentry sentry(is);
    if (!sentry)
    {
      return is;
    }

    std::string binStr;
    is >> binStr;
    if (is && binStr.substr(0, 2) != "0b" && binStr.substr(0, 2) != "0B")
    {
      is.setstate(std::ios::failbit);
      return is;
    }

    if (binStr.length() <= 2)
    {
      is.setstate(std::ios::failbit);
      return is;
    }

    if (binStr.substr(2).find_first_not_of("01") != std::string::npos)
    {
      is.setstate(std::ios::failbit);
      return is;
    }

    try
    {
      dest.ref = std::stoull(binStr.substr(2), nullptr, 2);
    }
    catch (...)
    {
      is.setstate(std::ios::failbit);
    }

    return is;
  }

  std::istream& operator>>(std::istream& is, StringIO&& dest)
  {
    std::istream::sentry sentry(is);
    if (!sentry)
    {
      return is;
    }

    return std::getline(is >> DelimiterIO{ '"' }, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& is, LabelIO&& dest)
  {
    std::istream::sentry sentry(is);
    if (!sentry)
    {
      return is;
    }

    std::string key;
    is >> key;

    if (is && key != dest.exp)
    {
      is.setstate(std::ios::failbit);
    }

    return is;
  }

  std::istream& operator>>(std::istream& is, DataStruct& data)
  {
    std::istream::sentry sentry(is);
    if (!sentry)
    {
      return is;
    }

    DataStruct in;

    {
      bool key1 = true, key2 = true, key3 = true;

      is >> DelimiterIO{ '(' };

      while ((key1 || key2 || key3) && is)
      {
        std::string fieldName;
        is >> fieldName;

        if (fieldName == ":key1" && key1)
        {
          is >> ULLLiteralIO{ data.key1 };
          key1 = false;
        }
        else if (fieldName == ":key2" && key2)
        {
          is >> ULLBinaryIO{ data.key2 };
          key2 = false;
        }
        else if (fieldName == ":key3" && key3)
        {
          is >> StringIO{ data.key3 };
          key3 = false;
        }
        else
        {
          is.setstate(std::ios::failbit);
          break;
        }

        if (is.peek() == ')') break;
        is >> DelimiterIO{ ':' };
      }
    }

    if (is)
    {
      data = in;
    }

    is >> DelimiterIO{ ')' };
    return is;
  }

  std::ostream& operator<<(std::ostream& os, const DataStruct& data)
  {
    std::ostream::sentry sentry(os);
    if (!sentry)
    {
      return os;
    }

    iofmtguard fmtguard(os);

    os << "(:"
      << "key1 " << data.key1 << "ull:"
      << "key2 0b";

    if (data.key2 == 0) {
      os << '0';
    }
    else {
      bool leading_zero = true;
      for (int i = 63; i >= 0; i--) {
        bool bit = (data.key2 >> i) & 1;
        if (bit) leading_zero = false;
        if (!leading_zero) {
          os << (bit ? '1' : '0');
        }
      }
    }

    os << ":key3 \"" << data.key3 << "\":)";

    return os;
  }

  iofmtguard::iofmtguard(std::basic_ios<char>& s)
    : s_(s), width_(s.width()), fill_(s.fill()),
    precision_(s.precision()), fmt_(s.flags()) {
  }

  iofmtguard::~iofmtguard() {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }
}
