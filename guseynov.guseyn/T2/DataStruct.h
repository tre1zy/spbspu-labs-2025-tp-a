#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>
#include <complex>
#include <utility>

struct DataStruct
{
  std::complex<double> key1;
  std::pair<long long, unsigned long long> key2;
  std::string key3;
};

std::istream& operator>>(std::istream& in, DataStruct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);
bool compareDataStruct(const DataStruct& a, const DataStruct& b);

#endif
