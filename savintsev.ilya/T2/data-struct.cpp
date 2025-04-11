#include "data-struct.hpp"
#include <iomanip>
#include "scope-guard.hpp"
#include "io-utils.hpp"

std::istream & savintsev::operator>>(std::istream & in, DataStruct & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  savintsev::DataStruct temp;
  bool has_key1 = false, has_key2 = false, has_key3 = false;

  try
  {
    in >> savintsev::DelimiterIO{'('};
    in >> savintsev::DelimiterIO{':'};

    for (size_t i = 0; i < 3; ++i)
    {
      LabelIO label{""};
      in >> label;

      if (label.exp_ == "key1")
      {
        in >> savintsev::DoubleIO{temp.key1};
        has_key1 = true;
      }
      else if (label.exp_ == "key2")
      {
        in >> savintsev::UllIO{temp.key2};
        has_key2 = true;
      }
      else if (label.exp_ == "key3")
      {
        in >> savintsev::StringIO{temp.key3};
        has_key3 = true;
      }
      else
      {
        throw std::runtime_error("Unknown key");
      }

      in >> savintsev::DelimiterIO{':'};
    }

    in >> savintsev::DelimiterIO{')'};

    if (!(has_key1 && has_key2 && has_key3))
    {
      throw std::runtime_error("Missing keys");
    }

    dest = temp;
  }
  catch (...)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream & savintsev::operator<<(std::ostream & out, const DataStruct & data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  savintsev::ScopeGuard guard(out);

  out << "(:";

  out << "key1 ";
  out << DoubleIO{const_cast< double & >(data.key1)} << ":";

  out << "key2 ";
  out << '0' << std::oct << data.key2 << ":";

  out << "key3 ";
  out << '"' << data.key3 << "\":)";

  return out;
}
