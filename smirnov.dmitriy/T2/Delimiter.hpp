#ifndef DELIMITER_I_HPP
#define DELIMITER_I_HPP

#include <istream>

namespace smirnov
{
    struct Delimiter
    {
        const char* expected;
    };

    std::istream& operator>>(std::istream&, Delimiter&&);
}

#endif
