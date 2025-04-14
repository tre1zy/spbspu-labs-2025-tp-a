#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "DataStruct.hpp"
namespace nikonov::detail
{
  bool sortComparator(DataStruct& a, DataStruct& b)
  {
    if (a.key1_ == b.key1_)
    {
      if (a.key2_ == b.key2_)
      {
        return a.key3_.length() > b.key3_.length();
      }
      return a.key2_ > b.key2_;
    }
    return a.key1_ > b.key1_;
  }
}
int main()
{
  // using nikonov::DataStruct;
  // using data_istream_it = std::istream_iterator< DataStruct >;
  // std::vector< DataStruct > dS;
  // std::copy(data_istream_it(std::cin), data_istream_it(), std::back_inserter(dS));
  // std::sort(dS.begin(), dS.end(), nikonov::detail::sortComparator);
  // std::copy(dS.begin(), dS.end(), std::ostream_iterator< DataStruct >(std::cout, "\n"));

  using nikonov::DataStruct;
  DataStruct dS;
  std::cin >> dS;
  std::cout << dS << '\n';
}
