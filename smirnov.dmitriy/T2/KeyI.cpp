#include "KeyI.hpp"
#include <istream>
#include <bitset>
#include "DelimiterI.hpp"

std::istream& smirnov::operator>>(std::istream& in, BinKey&& key)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        in.setstate(std::ios::failbit);
    }
    std::bitset< 64 > bin;
    in >> strictDel{ '0' } >> anyDel{ 'b' };
    in >> bin;
    if (in)
    {
        key.value = bin.to_ullong();
    }
    return in;
}
