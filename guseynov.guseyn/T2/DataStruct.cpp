#include "DataStruct.h"
#include <iomanip>

std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
    out << "(:key1 #c(" << data.key1.real() << " " << data.key1.imag() << "):key2 0x" << std::hex << std::uppercase << data.key2 << std::dec << ":key3 \"" << data.key3 << "\":)";
    return out;
}
