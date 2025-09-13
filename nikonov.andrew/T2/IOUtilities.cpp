#include "IOUtilities.hpp"
#include <stdexcept>
#include <cmath>
#include <string>
#include <iomanip>
#include <queue>
#include "StreamGuard.hpp"
namespace nikonov
{
  namespace detail
  {
    int hexCharToInt(char c)
    {
      if (c >= 'A' && c <= 'F')
      {
        return c - 'A' + 10;
      }
      if (c >= 'a' && c <= 'f')
      {
        return c - 'a' + 10;
      }
      return c;
    }
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry)
      {
        return in;
      }
      char c = '0';
      in >> c;
      if (in && (c != dest.exp))
      {
        in.setstate(std::ios::failbit);
      }
      return in;
    }
    std::istream& operator>>(std::istream& in, DoubleI&& dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry)
      {
        return in;
      }
      double val = 0.0;
      if (!(in >> val))
      {
        return in;
      }
      double abs_val = abs(val);
      if (abs_val == 0.0)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      int exp = static_cast<int>(std::floor(std::log10(abs_val)));
      double mantissa = abs_val / std::pow(10, exp);
      if (!(mantissa >= 1.0 && mantissa < 10.0))
      {
          in.setstate(std::ios::failbit);
          return in;
      }
      dest.ref = val;
      return in;
    }
    std::istream& operator>>(std::istream& in, UnsignedLongLongI&& dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry)
      {
        return in;
      }
      in >> DelimiterIO{ '0' };
      char c = '0';
      if (in >> c && (c != 'x' && c != 'X'))
      {
        in.setstate(std::ios::failbit);
      }
      in >> std::hex >> dest.ref;
      if (!in)
      {
        in.setstate(std::ios::failbit);
      }
      return in;
    }
    std::istream& operator>>(std::istream& in, StringI&& dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry)
      {;
        return in;
      }
      std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
      return in;
    }
    std::istream& operator>>(std::istream& in, LabelI&& dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry)
      {
        return in;
      }
      std::getline(in, dest.ref, ' ');
      return in;
    }

    std::ostream& operator<<(std::ostream& out, const DoubleO& dest)
    {
      std::ostream::sentry sentry(out);
      if (!sentry)
      {
        return out;
      }
      StreamGuard outGuard(out);
      int exp = 0;
      double toOut = dest.ref;
      while (toOut >= 10)
      {
        toOut /= 10;
        ++exp;
      }
      while (toOut < 1)
      {
        toOut *= 10;
        --exp;
      }
      out << std::fixed << std::setprecision(1) << toOut << 'e';
      if (exp > 0)
      {
        out << '+';
      }
      out << exp;
      return out;
    }
    std::ostream& operator<<(std::ostream& out, const UnsignedLongLongO& toOut)
    {
      std::ostream::sentry sentry(out);
      if (!sentry)
      {
        return out;
      }
      out << "0x" << std::hex << std::uppercase <<  toOut.ref;
      return out;;
    }
    std::ostream& operator<<(std::ostream& out, const StringO& toOut)
    {
      std::ostream::sentry sentry(out);
      if (!sentry)
      {
        return out;
      }
      out << '"' << toOut.ref << '"';
      return out;
    }
  }
}
