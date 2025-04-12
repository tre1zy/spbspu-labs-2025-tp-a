#include <iostream>
#include "data_struct.hpp"

int main()
{
  rychkov::DataStruct ds;
  if (!(std::cin >> ds))
  {
    std::cerr << "fail\n";
  }
  std::cout << ds.key1 << ' ' << ds.key2 << ' ' << ds.key3 << '\n';
}
