#include "data-struct.hpp"

std::istream & savintsev::operator>>(std::istream & in, DataStruct & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct input;
  bool has_key1 = false, has_key2 = false, has_key3 = false;

  try
  {
    in >> DelimiterIO{'{'};
    bool first = true;

    while (in.peek() != '}')
    {
      if (!first)
      {
        in >> DelimiterIO{','};
      }
      first = false;

      std::string label;
      if (!(in >> LabelIO{label}))
      {
        throw std::runtime_error("Expected key");
      }

      in >> DelimiterIO{':'};

      if (label == "key1") {
        if (!(in >> DoubleIO{input.key1}))
        {
          throw std::runtime_error("Failed to read key1");
        }
        has_key1 = true;
      }
      else if (label == "key2")
      {
        if (!(in >> UllIO{input.key2}))
        {
          throw std::runtime_error("Failed to read key2");
        }
        has_key2 = true;
      }
      else if (label == "key3")
      {
        if (!(in >> StringIO{input.key3}))
          throw std::runtime_error("Failed to read key3");
        has_key3 = true;
      }
      else
      {
        throw std::runtime_error("Unknown key: " + label);
      }
    }

    in >> DelimiterIO{'}'};

    if (!(has_key1 && has_key2 && has_key3))
    {
      throw std::runtime_error("Missing required key(s)");
    }

    dest = input;
  }
  catch (...)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}
