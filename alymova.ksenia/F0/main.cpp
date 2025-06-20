#include <iostream>
#include <fstream>
#include "commands.hpp"

int main(int argc, char** argv)
{
  if (argc > 2)
  {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }
  std::istream* input = &std::cin;
  std::ifstream file;
  if (argc == 2)
  {
    if (argv[1] == "--help")
    {
      return 0;
    }
    file.open(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "<INCORRECT FILE>\n";
      return 1;
    }
    input = &file;
  }
  
}

