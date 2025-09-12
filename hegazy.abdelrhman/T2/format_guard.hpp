
#ifndef FORMAT_GUARD_HPP
#define FORMAT_GUARD_HPP

#include <ios>

namespace martynov {

    class FormatGuard {
    private:
        std::ios& stream;
        std::ios::fmtflags flags;

    public:
        FormatGuard(std::ios& s) : stream(s), flags(s.flags()) {}
        ~FormatGuard() { stream.flags(flags); }
    };

} 

#endif  