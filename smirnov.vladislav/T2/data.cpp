#include "data.hpp"
#include "stream_guard.hpp"
#include <iomanip>
#include <sstream>
#include <cmath>

namespace
{
    using namespace smirnov::ioStructs;
}

std::istream& smirnov::ioStructs::operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    char c;
    in >> c;
    if (in && c != dest.expected)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& smirnov::ioStructs::operator>>(std::istream& in, DoubleIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    std::string str;
    std::getline(in, str, ':');

    size_t exp_pos = str.find_first_of("eE");
    if (exp_pos == std::string::npos ||
        exp_pos == str.length() - 1 ||
        (str[exp_pos + 1] != '+' &&
         str[exp_pos + 1] != '-'))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::istringstream iss(str);
    if (!(iss >> dest.ref))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}


std::istream& smirnov::ioStructs::operator>>(std::istream& in, ULLIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    std::string str;
    if (!(in >> str) || (str.substr(0, 2) != "0x" && str.substr(0, 2) != "0X"))
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    str = str.substr(2);
    std::istringstream iss(str);
    iss >> std::hex >> dest.ref;
    if (!iss)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}


std::istream& smirnov::ioStructs::operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    return in;
}



std::istream& smirnov::ioStructs::operator>>(std::istream& in, KeyNumIO& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    std::string word = "";
    in >> word;
    if (word.substr(0, 3) != "key")
    {
        in.setstate(std::ios::failbit);
    }
    dest.exp = word.back();
    return in;
}

std::istream& smirnov::operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    DataStruct temp;
    {
        using del = DelimiterIO;
        using keynum = KeyNumIO;
        using ullval = ULLIO;
        using dbval = DoubleIO;
        using str = StringIO;

        in >> del{ '(' };
        in >> del{ ':' };

        for (int i = 0; i < 3; ++i)
        {
            keynum key_num{ 0 };
            in >> key_num;
            if (key_num.exp == '1')
            {
                in >> dbval{ temp.key1 } >> del{ ':' };
            }
            else if (key_num.exp == '2')
            {
                in >> ullval{ temp.key2 } >> del{ ':' };
            }
            else if (key_num.exp == '3')
            {
                in >> str{ temp.key3 } >> del{ ':' };
            }
        }
        in >> del{ ')' };
    }
    if (in)
    {
        dest = temp;
    }
    return in;
}



std::ostream& smirnov::ioStructs::operator<<(std::ostream& out, const DoubleIO&& dest)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
    smirnov::iofmtguard fmtguard(out);

    double value = std::abs(dest.ref);
    bool is_negative = (dest.ref < 0);
    int exponent = (value == 0.0) ? 0 : static_cast<int>( std::floor(std::log10(value)));
    double mantissa = value / std::pow(10, exponent);

    if (mantissa >= 10.0)
    {
        mantissa /= 10.0;
        exponent++;
    }
    else if (mantissa < 1.0)
    {
        mantissa *= 10.0;
        exponent--;
    }

    if (is_negative)
    {
        out << '-';
    }

    out << std::fixed << std::setprecision(1) << mantissa;
    out << 'e' << (exponent >= 0 ? '+' : '-') << std::abs(exponent);
    return out;
}

std::ostream& smirnov::ioStructs::operator<<(std::ostream& out, const ULLIO&& dest)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
    return out << "0X" << std::uppercase << std::hex << dest.ref;
}

std::ostream& smirnov::operator<<(std::ostream& out, const DataStruct& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }

    smirnov::iofmtguard guard(out);
    double dbval = src.key1;
    unsigned long long ullval = src.key2;

    out << "(:key1 " << DoubleIO{ dbval };
    out << ":key2 " << ULLIO{ ullval };
    out << ":key3 \"" << src.key3;
    out << "\":)";
    return out;
}
