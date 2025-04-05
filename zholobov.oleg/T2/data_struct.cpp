#include "data_struct.hpp"

#include <iostream>

namespace zholobov {

  namespace io_helpers {

    struct DelimiterIO {
      char c;
    };

    struct LongLongIO {
      long long& ref;
    };

    struct PairIO {
      std::pair< long long, unsigned long long >& ref;
    };

    struct StringIO {
      std::string& ref;
    };

    std::istream& operator>>(std::istream& input, DelimiterIO&& value);
    std::istream& operator>>(std::istream& input, LongLongIO&& value);
    std::istream& operator>>(std::istream& input, PairIO&& value);
    std::istream& operator>>(std::istream& input, StringIO&& value);

    std::ostream& operator<<(std::ostream& output, const LongLongIO& value);
    std::ostream& operator<<(std::ostream& output, const PairIO& value);
    std::ostream& operator<<(std::ostream& output, const StringIO& value);

  }
}

std::istream& zholobov::io_helpers::operator>>(std::istream& input, DelimiterIO&& value)
{
  std::istream::sentry s(input);
  if (!s) {
    return input;
  }
  char c = 0;
  input >> c;
  if (input && (std::tolower(c) != value.c)) {
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& zholobov::io_helpers::operator>>(std::istream& input, LongLongIO&& value)
{
  std::istream::sentry s(input);
  if (!s) {
    return input;
  }
  long long temp = 0;
  if (!(input >> temp >> DelimiterIO{'l'} >> DelimiterIO{'l'})) {
    return input;
  }
  value.ref = temp;
  return input;
}

std::ostream& zholobov::io_helpers::operator<<(std::ostream& output, const LongLongIO& value)
{
  std::ostream::sentry s(output);
  if (!s) {
    return output;
  }
  output << value.ref << "ll";
  return output;
}

std::istream& zholobov::io_helpers::operator>>(std::istream& input, PairIO&& value)
{
  std::istream::sentry s(input);
  if (!s) {
    return input;
  }
  long long n = 0;
  unsigned long long d = 0;
  input >> DelimiterIO{'N'} >> n >> DelimiterIO{':'} >> DelimiterIO{'D'} >> d;
  if (input) {
    value.ref = std::make_pair(n, d);
  }
  return input;
}

std::ostream& zholobov::io_helpers::operator<<(std::ostream& output, const PairIO& value)
{
  std::ostream::sentry s(output);
  if (!s) {
    return output;
  }
  output << "N " << value.ref.first << ":D " << value.ref.second;
  return output;
}

std::istream& zholobov::io_helpers::operator>>(std::istream& input, StringIO&& value)
{
  std::istream::sentry s(input);
  if (!s) {
    return input;
  }
  std::string temp;
  input >> DelimiterIO{'"'};
  std::getline(input, temp, '"');
  if (input) {
    value.ref = temp;
  }
  return input;
}

std::ostream& zholobov::io_helpers::operator<<(std::ostream& output, const StringIO& value)
{
  std::ostream::sentry s(output);
  if (!s) {
    return output;
  }
  output << '"' << value.ref << '"';
  return output;
}

std::istream& zholobov::operator>>(std::istream& input, DataStruct& value)
{
  std::istream::sentry s(input);
  if (!s) {
    return input;
  }
  DataStruct temp{};
  input >> io_helpers::DelimiterIO{'('} >> io_helpers::DelimiterIO{':'};
  for (int i = 0; i < 3; ++i) {
    std::string key;
    input >> key;
    if (input && (key.size() == 4) && key.substr(0, 3) == "key") {
      char k = key.back();
      switch (k) {
        case '1':
          input >> io_helpers::LongLongIO{temp.key1};
          break;
        case '2':
          input >> io_helpers::DelimiterIO{'('} >> io_helpers::DelimiterIO{':'};
          input >> io_helpers::PairIO{temp.key2};
          input >> io_helpers::DelimiterIO{':'} >> io_helpers::DelimiterIO{')'};
          break;
        case '3':
          input >> io_helpers::StringIO{temp.key3};
          break;
        default:
          input.setstate(std::ios::failbit);
      }
    } else {
      input.setstate(std::ios::failbit);
    }
    input >> io_helpers::DelimiterIO{':'};
  }
  input >> io_helpers::DelimiterIO{')'};
  if (input) {
    value = temp;
  }
  return input;
}

std::ostream& zholobov::operator<<(std::ostream& output, const DataStruct& value)
{
  std::ostream::sentry s(output);
  if (!s) {
    return output;
  }
  long long ll = value.key1;
  std::pair< long long, unsigned long long > pair = value.key2;
  std::string str = value.key3;
  output << "(:key1 " << io_helpers::LongLongIO{ll};
  output << ":key2 (:" << io_helpers::PairIO{pair} << ":)";
  output << ":key3 " << io_helpers::StringIO{str} << ":)";
  return output;
}
