#include "DataStruct.hpp"
#include <iomanip>
#include "StreamGuard.hpp"
#include "IOUtilities.hpp"
namespace nikonov
{
  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    DataStruct input;
    {
      using sep = detail::DelimiterIO;
      using label = detail::LabelI;
      using dbl = detail::DoubleI;
      using ull = detail::UnsignedLongLongI;
      using str = detail::StringI;
      bool hasKey1 = false;
      bool hasKey2 = false;
      bool hasKey3 = false;
      in >> sep{ '(' } >> sep{ ':' };
      for (size_t i = 0; i < 3; ++i)
      {
        std::string keyName;
        in >> label{ keyName };
        if (keyName == "key1")
        {
          if (hasKey1)
          {
            in.setstate(std::ios::failbit);
            break;
          }
          in >> dbl{ input.key1 } >> sep{ ':' };
          hasKey1 = true;
        }
        else if (keyName == "key2")
        {
          if (hasKey2)
          {
            in.setstate(std::ios::failbit);
            break;
          }
          in >> ull{ input.key2 } >> sep{ ':' };
          hasKey2 = true;
        }
        else if (keyName == "key3")
        {
          if (hasKey3)
          {
            in.setstate(std::ios::failbit);
            break;
          }
          in >> str{ input.key3 } >> sep{ ':' };
          hasKey3 = true;
        }
        else
        {
          in.setstate(std::ios::failbit);
        }
      }
      in >> sep{ ')' };
      if (!(hasKey1 && hasKey2 && hasKey3))
      {
        in.setstate(std::ios::failbit);
      }
    }
    if (in)
    {
      dest = input;
    }
    return in;
  }
  std::ostream& operator<<(std::ostream& out, const DataStruct& toOut)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard outGuard(out);
    out << "(";
    out << ":key1 " << detail::DoubleO{ toOut.key1 };
    out << ":key2 " << detail::UnsignedLongLongO{ toOut.key2 };
    out << ":key3 " << detail::StringO{ toOut.key3 };
    out << ":)";
    return out;
  }
}
