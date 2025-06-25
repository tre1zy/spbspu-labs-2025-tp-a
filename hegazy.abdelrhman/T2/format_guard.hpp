#ifndef FORMAT_GUARD_HPP
#define FORMAT_GUARD_HPP

#include <iostream>
#include <ios>

namespace pilugina
{
    class FormatGuard
    {
    public:
        explicit FormatGuard(std::ostream& s);
        ~FormatGuard();

    private:
        std::ostream& stream_;
        std::ios::fmtflags flags_;
        std::streamsize precision_;
        char fill_;
    };
}

#endif