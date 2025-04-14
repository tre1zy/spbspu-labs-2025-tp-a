#include "data.hpp"
#include "stream_guard.hpp"
#include <iomanip>
#include <sstream>
#include <cmath>

namespace smirnov
{

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
	std::istream::sentry sentry(in);
	if (!sentry)
	{
            return in;
        }
        char c = '0';
        in >> c;
        if (in && c != dest.expected)
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

        return in >> dest.ref;
    }

    std::istream& operator>>(std::istream& in, UllIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        std::string str;
        in >> std::ws;
        char a = '0';
        char b = '0';
        a = in.get();
        a = in.peek();

        if (a == '0' && (b == 'x' || b == 'X'))
        {
            in.get();
            in >> std::hex >> dest.ref;
            if (!in)
            {
                in.setstate(std::ios::failbit);
            }
        }
        else
        {
            in.unget();
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
        std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
        return in;
    }

    std::istream& operator>>(std::istream& in, KeyNumIO& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string data;
        if ((in >> data) && (data != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        DataStruct str;
        bool hasKey1 = false;
        bool hasKey2 = false;
        bool hasKey3 = false;
        if (!(in >> DelimiterIO{ '(' }))
        {
            return in;
        }

        while (true)
        {
            in >> std::ws;
            if (in.peek() == ':')
            {
                in.get();
                if (in.peek() == ')')
                {
                    in.get();
                    break;
                }
                else
                {
                    in.unget();
                }
            }
            if (!(in >> DelimiterIO{ ':' }))
            {
                in.setstate(std::ios::failbit);
                return in;
            }
            std::string key;
            if (!(in >> key))
            {
                in.setstate(std::ios::failbit);
                return in;
            }
            if (key == "key1")
            {
                if (!(in >> DoubleIO{ str.key1 }))
                {
                    return in;
                }
                hasKey1 = true;
            }
            else if (key == "key2")
            {
                if (!(in >> UllIO{ str.key2 }))
                {
                    return in;
                }
                hasKey2 = true;
            }
            else if (key == "key3")
            {
                if (!(in >> StringIO{ str.key3 }))
                {
                    return in;
                }
                hasKey3 = true;
            }
            else
            {
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        if (hasKey1 && hasKey2 && hasKey3)
        {
            dest = str;
        }
        else
        {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DoubleIO&& dest)
    {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        iofmtguard guard(out);

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

    std::ostream& operator<<(std::ostream& out, const UllIO&& dest)
    {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        return out << "0x" << std::uppercase << std::hex << dest.ref;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        double dbval = 0;
        unsigned long long ullval = 0;
        dbval = src.key1;
        ullval = src.key2;
        iofmtguard guard(out);
        out << "(:key1 " << smirnov:: DoubleIO{ dbval }
            << ":key2 " << smirnov: UllIO{ ullval };
            << ":key3 \"" << src.key3 << "\":)";

        return out;
    }

    bool operator<(const DataStruct& a, const DataStruct& b)
    {
        if (a.key1 != b.key1)
        {
            return a.key1 < b.key1;
        }

        if (a.key2 != b.key2)
        {
            return a.key2 < b.key2;
        }
        return a.key3 < b.key3;
    }
}
