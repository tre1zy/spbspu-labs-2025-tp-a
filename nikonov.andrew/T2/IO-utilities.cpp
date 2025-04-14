#include "IO-utilities.hpp"
#include <stdexcept>
#include <cmath>
#include <string>
#include <stack>
namespace nikonov
{
  namespace detail
  {
    int hexCharToInt(char c)
    {
      if (c == 'A' || c == 'a')
      {
        return 10;
      }
      else if (c == 'B' || c == 'b')
      {
        return 11;
      }
      else if (c == 'C' || c == 'c')
      {
        return 12;
      }
      else if (c == 'D' || c == 'd')
      {
        return 13;
      }
      else if (c == 'E' || c == 'e')
      {
        return 14;
      }
      else if (c == 'F' || c == 'f')
      {
        return 15;
      }
      std::string str;
      str += c;
      return std::stoi(str);
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
      if (in && (c != dest.exp_))
      {
        in.setstate(std::ios::failbit);
      }
      return in;
    }
    std::istream& operator>>(std::istream& in, DoubleIO&& dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry)
      {
        return in;
      }
      char c = '0';
      std::string str;
      try
      {
        while (in >> c && (c != 'e' && c != 'E'))
        {
          str += c;
        }
        double mantissa = std::stod(str);
        str.clear();
        while (in >> c && (c != ':'))
        {
          str += c;
        }
        int exp = std::stoi(str);
        dest.ref_ = mantissa * std::pow(10, exp);
      }
      catch (const std::exception& e)
      {
        in.setstate(std::ios::failbit);
      }
      return in;
    }
    std::istream& operator>>(std::istream& in, UnsignedLongLong&& dest)
    {
      in >> DelimiterIO{ '0' };
      char c = '0';
      if (in >> c && (c != 'x' && c != 'X'))
      {
        in.setstate(std::ios::failbit);
      }
      try
      {
        size_t i = 0;
        unsigned long long tempUll = 0;
        std::stack< int > digits;
        while (in >> c && (c != ':'))
        {
          digits.push(hexCharToInt(c));
          i++;
        }
        for (; i > 0; i--)
        {
          tempUll += digits.top() * std::pow(16, i - 1);
          digits.pop();
        }
        dest.ref_ = tempUll;
      }
      catch(const std::exception& e)
      {
        in.setstate(std::ios::failbit);
      }
      return in;
    }
    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry)
      {
        return in;
      }
      return std::getline(in >> DelimiterIO{ '"' }, dest.ref_, '"');
    }
    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
      std::istream::sentry sentry(in);
      if (!sentry)
      {
        return in;
      }
      std::string data = "";
      if ((in >> StringIO{ data }) && (data != dest.exp_))
      {
        in.setstate(std::ios::failbit);
      }
      return in;
    }

    std::ostream& operator<<(std::ostream& out, DoubleIO&& dest)
    {
      std::ostream::sentry sentry(out);
      if (!sentry)
      {
        return out;
      }
      int exp = 0;
      double toOut = dest.ref_;
      for (; toOut > 10; exp++)
      {
        toOut /= 10;
      }
      out << toOut << 'e' << exp;
      return out;
    }
    std::ostream& operator<<(std::ostream& out, UnsignedLongLong&& toOut)
    {
      std::ostream::sentry sentry(out);
      if (!sentry)
      {
        return out;
      }
      out << "0x" << std::hex << toOut.ref_;
      return out;;
    }
    std::ostream& operator<<(std::ostream& out, StringIO&& toOut)
    {
      std::ostream::sentry sentry(out);
      if (!sentry)
      {
        return out;
      }
      out << toOut.ref_;
      return out;
    }
  }
}
