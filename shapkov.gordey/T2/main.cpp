#include <iostream>
#include "types.hpp"
#include <sstream>

int main()
{
  //using dsio = shapkov::doubleScientificIO;
  shapkov::ratioIO param;
  std::istringstream input("(:N -23 :D 353  :)");
  input >> param;
  if (input)
  {
    std::cout << param.key.first << " " << param.key.second << '\n';
  }
  else
  {
    std::cout << ":(\n";
  }
}
