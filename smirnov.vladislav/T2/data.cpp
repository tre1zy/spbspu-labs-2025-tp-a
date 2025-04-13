#include "data.h"
#include "stream_guard.h"
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

    std::string num;
    std::getline(in, num, ':');  

    size_t ePos = num.find_first_of("eE");
    if (ePos == std::string::npos || num[ePos] == num.back())
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (num[ePos + 1] != '+' && num[ePos + 1] != '-')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::istringstream iss(num);
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

    double val = dest.ref;
    bool is_negative = val < 0;
    val = std::abs(val);
    int exp = (val == 0.0) ? 0 : std::floor(std::log10(val));
    double mant = val / std::pow(10, exp);

    if (mant >= 10.0)
    {
        mant /= 10.0;
        exp++;
    }
    else if (mant < 1.0)
    {
        mant *= 10.0;
        exp--;
    }

    if (is_negative)
    {
        out << '-';
    }

    out << std::fixed << std::setprecision(1) << mant;
    out << 'e' << (exp >= 0 ? '+' : '-') << std::abs(exp);
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
