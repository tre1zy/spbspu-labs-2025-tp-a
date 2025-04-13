#ifndef STREAM_GUARD_H
#define STREAM_GUARD_H
#include <iostream>

namespace smirnov
{
    class iofmtguard
    {
    public:
        iofmtguard(std::basic_ios<char>& stream);
        ~iofmtguard();

    private:
        std::basic_ios<char>& stream_;
        std::streamsize precision_;
        std::streamsize width_;
        char fill_;
        std::ios::fmtflags flags_;
    };
}

#endif
