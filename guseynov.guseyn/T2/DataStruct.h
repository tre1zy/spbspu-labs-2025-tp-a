#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <complex>
#include <string>

struct DataStruct
{
    std::complex<double> key1;
    unsigned long long key2;
    std::string key3;
};

std::ostream& operator<<(std::ostream& out, const DataStruct& data);

#endif
