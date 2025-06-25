#include "format_wrapper.hpp"
#include "delimiter.hpp"
#include "utils.hpp"
#include <iostream>
#include <cctype>

std::istream& pilugina::operator>>(std::istream& in, UnsignedLongLongLIT&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    unsigned long long value = 0;
    std::string input;
    char c = 0;

    while (in.get(c) && std::isdigit(c))
    {
        input += c;
    }

    if (!input.empty())
    {
        try
        {
            size_t pos = 0;
            value = std::stoull(input, &pos);
            if (pos != input.size())
            {
                in.setstate(std::ios::failbit);
            }
            else
            {
                dest.ref = value;
            }
        }
        catch (...)
        {
            in.setstate(std::ios::failbit);
        }
    }
    else
    {
        in.setstate(std::ios::failbit);
    }

    if (in)
    {
        in.putback(c);
    }

    return in;
}

std::istream& pilugina::operator>>(std::istream& in, UnsignedLongLongBIN&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    in >> DelimiterIO{ '0' } >> DelimiterIO{ 'b' };
    if (!in)
    {
        return in;
    }

    unsigned long long value = 0;
    char c = 0;
    std::string bin;

    while (in.get(c) && isBinaryDigit(c))
    {
        bin += c;
    }

    if (!bin.empty())
    {
        try
        {
            size_t pos = 0;
            value = std::stoull(bin, &pos, 2);
            if (pos != bin.size())
            {
                in.setstate(std::ios::failbit);
            }
            else
            {
                dest.ref = value;
            }
        }
        catch (...)
        {
            in.setstate(std::ios::failbit);
        }
    }
    else
    {
        in.setstate(std::ios::failbit);
    }

    if (in)
    {
        in.putback(c);
    }

    return in;
}

std::istream& pilugina::operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    in >> DelimiterIO{ '"' };
    if (!in)
    {
        return in;
    }

    std::string str;
    char c = 0;
    while (in.get(c) && c != '"')
    {
        str += c;
    }

    if (in && c == '"')
    {
        dest.ref = str;
    }
    else
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream& pilugina::output::operator<<(std::ostream& out, const UnsignedLongLongLIT& dest)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }

    out << dest.ref << "ull";
    return out;
}

std::ostream& pilugina::output::operator<<(std::ostream& out, const UnsignedLongLongBIN& dest)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }

    std::string bin;
    unsigned long long value = dest.ref;
    if (value == 0)
    {
        bin = "0";
    }
    else
    {
        while (value > 0)
        {
            bin = (value % 2 == 0 ? "0" : "1") + bin;
            value /= 2;
        }
    }

    out << "0b" << bin;
    return out;
}