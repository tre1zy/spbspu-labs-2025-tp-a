#include <iostream>
#include "types.hpp"
#include <sstream>
#include <delimiter.hpp>
#include "dataStruct.hpp"

int main()
{
  shapkov::DataStruct data;
  //std::istringstream input("(:N -23 :D 36  :)");
  //std::istringstream input1("5.45d");
  //std::istringstream input2("\"Data\":");
  //std::istringstream input3("(:N -23 :D 36  :) 5.45 \"Data\"");
  //input3 >> shapkov::ratioIO{ data.key2 } >> shapkov::doubleScientificIO{ data.key1 } >> shapkov::stringIO{ data.key3 };
  std::cin >> data;
  std::cout << data << '\n';
  std::cout << data.key1 << '\n';
  //std::istringstream input4("key r");
  //input4 >> shapkov::labelIO{ "key" } >> shapkov::delimiterIO{ 'f' };
  if (std::cin)
  {
    std::cout << "all right\n";
  }
  else
  {
    std::cout << "bad job\n";
  }
}
