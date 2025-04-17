#include "output_wrapper_structs.hpp"
#include "format_guard.hpp"
#include <cmath>

int puzikov::output::calcBitWidth(unsigned long long num)
{
  if (num == 0)
  {
    return 1;
  }
  return static_cast< int >(std::log2(num)) + 1;
}

void puzikov::output::fillBinRepresentationString(unsigned long long num, std::string &binString)
{
  int bitWidth = calcBitWidth(num);
  binString.resize(2 + bitWidth);
  binString[0] = '0';
  binString[1] = 'b';

  for (int i = bitWidth - 1; i >= 0; --i)
  {
    binString[2 + i] = (num % 2) ? '1' : '0';
    num /= 2;
  }
}

std::ostream &puzikov::output::operator<<(std::ostream &out, const ULLValue &source)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard guard(out);

  if (source.ref == 1)
  {
    return out << "0b01";
  }
  
  std::string binString;
  fillBinRepresentationString(source.ref, binString);
  return out << binString;
}

std::ostream &puzikov::output::operator<<(std::ostream &out, const PairValue &source)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard guard(out);
  return out << "(:N " << source.ref.first << ":D " << source.ref.second << ":)";
}

std::ostream &puzikov::output::operator<<(std::ostream &out, const StringValue &source)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard guard(out);
  return out << '\"' << source.ref << '\"';
}

