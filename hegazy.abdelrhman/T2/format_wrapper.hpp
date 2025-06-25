#ifndef WRAPPERS_HPP
#define WRAPPERS_HPP

#include <string>

namespace pilugina
{
    struct UnsignedLongLongLIT
    {
        unsigned long long& ref;
    };
    struct UnsignedLongLongBIN
    {
        unsigned long long& ref;
    };
    struct StringIO
    {
        std::string& ref;
    };

    std::istream& operator>>(std::istream& in, UnsignedLongLongLIT&& dest);
    std::istream& operator>>(std::istream& in, UnsignedLongLongBIN&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);

    namespace output
    {
        struct UnsignedLongLongLIT
        {
            const unsigned long long& ref;
        };
        struct UnsignedLongLongBIN
        {
            const unsigned long long& ref;
        };
        std::ostream& operator<<(std::ostream& out, const UnsignedLongLongLIT& dest);
        std::ostream& operator<<(std::ostream& out, const UnsignedLongLongBIN& dest);
    }
}

#endif